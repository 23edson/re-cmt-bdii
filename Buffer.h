/*Este trabalho utiliza a licença GNU General Public License.
* gcc (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3
* Copyright (C) 2011 Free Software Foundation, Inc. 
* 
* Seu objetivo é criar tabelas com seus respectivos atributos, 
* armazenando-os em disco. Assim, o programa permite gravar em arquivo
* as tuplas que pertencem a uma dada tabela. 
* Possui um bufferPool que controla as cópias dos dados em disco para
* a memória. 
* 
* Implementação : 
* 
* 		Primeira parte : Raphael Machado dos Santos;
*		Segunda parte  : Caciano Martinelli de marco, Mateus Trebien, Thiago Mendes da Silva;
*		Terceira parte : Edson Lemes da Silva, Lucas Cezar Parnoff.
* 
**/


/** 
 * 
 * Bibliotecas necessárias para o funcionamento do programa 
 * 
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "erro.h"

/**
 * defines 
 * 	- TNAME_LENGHT - Nome de um atributo pode ter tamanho máximo conforme definido aqui;
 *  - BP_PAGES - Número de páginas que o bufferPool possui; 
 *  - BP_SIZE - Tamanho em bytes que uma página pode armazenar de dados; 
 **/
#define TNAME_LENGHT 20
#define BP_PAGES 16
#define BP_SIZE 512


/**
 *@struct field - Descreve um atributo de uma tabela qualquer. 
 * char fName - Nome dado ao atributo em questão;
 * char fType - O tipo dele, estando disponíveis ( I, D, S, C),
 * int, double, string, caracter na respectiva ordem.
 * 
 * int fLenght - Tamanho do atributo, importante considerar que
 * para os tipos double e caracter o tamanho é ignorado, uma vez que 
 * o tamanho de double é 8bytes, e caracter é 1 byte. No caso de um
 * número inteiro, fLenght significa a quantidade de dígitos que aquele
 * atributo suportará. Ex : fLenght = 2 , aceitará número de 0 até 99.
 **/
typedef struct field{
    char fName[TNAME_LENGHT];
    char fType;
    int fLenght;
}field;


/**
 *@struct bufferPage - Refere-se a uma página do buffer;
 * 
 * int idNumber - Número de identificação de uma tabela, este id
 * é recuperado do arquivo em disco. Deste modo, uma página possui
 * tuplas apenas da mesma tabela.
 * 
 * int countTuples - Contagem do número de tuplas que estão nesta página;
 * field *fieldList - Lista de atributos. Em outras palavras, são os metadados
 * da tabela. Esta variável é setada quando a primeira tupla é inserida na page;
 * 
 * int diskSeek - Quantidade de bytes ocupados;
 * int pinCount - Se a página está em uso. Estará em zero (Não está em uso ), ou
 * em um (Está sendo usada);
 * 
 * int rewriteBit- se está em um significa que a página pode ir pro disco
 * char *data - Nessa string estarão os dados carregados dos arquivos. As tuplas;
 **/
typedef struct bufferPage
{
	int idNumber;
	int countTuples;
	field *fieldList;
	int fieldCount;
	int diskSeek; 
	int pinCount; //Pin Count
	int rewriteBit;//Dirty Bit
	char *data; //Tupla de dados
}bufferPage;

/**
 * @struct buffer - Parte geral do bufferPool, aqui estão definidas 
 * informações globais sobre as páginas do buffer.
 * 
 * int countItems - Total de tuplas contida nas páginas.
 * int countMeta  - Quantidade de metadados presentes no buffer, ou ainda, 
 * número de tabelas que possuem pelo menos uma tupla carregada do disco;
 * int nextPageAvaliable - Armazena a próxima página livre para inserção;
 * bufferPage bp[] - Vetor de páginas disponíveis;
 **/
typedef struct buffer
{
	int countItems;
	int countMeta;
	int nextPageAvaliable;
	//bufferPage *itens;
	bufferPage bp[BP_PAGES];
}buffer;

/*typedef struct tupla{
	char tipo;
	char *String;
	int *Dint;
	double *Ddouble;
}myRow;*/

/**
 * @struct Elemento - Representa um dado, que eventualmente será extraído do buffer.
 * @enum ElementType - Possui quatro tipos possíveis ( String, int, double, caracter)
 * @union - Representa um dado conforme o tipo definido em ElementType;
 **/
typedef struct Elemento {
  enum ElementType { String, Nint, Ndouble,Caracter }type;
  union {
    char      *Str;
    int       *Dint;
    double    *Ddouble;
  };
}Element_t;

//Inicializa o buffer
void initBuffer(buffer *bPool);

//Procura a próxima página disponível
void findNextAvaliable(buffer *bPool);

//Aplica as políticas de troca
void applyReplacementPolicies(buffer *bPool);

//Insere um novo elemento no buffer

int bufferInsert(buffer *bPool,char *tupla,int tupleLenght, field *fieldList, int fieldCount, int id);

//Função que vai ler os arquivos de dados e metadados

/**
 * O objetivo dessa função é recuperar uma tupla de uma tabela em disco, e
 * carregá-lo para o bufferPool;
 * 
 * @param buffer **bufferPool - Recebe o endereço de um ponteiro do tipo bufferPool, esse ponteiro 
 * deve ser NULO ( Na primeira chamada ) , pois a função vai se encarregar de inicializá-lo devidamente.
 * 
 * @param char *nomeTabela - Nome lógico da tabela de qual a tupla deve ser recuperada.
 * @param int contador - Este contador representa o número da tupla em que deseja-se trazer para o buffer. O
 * contador basea-se na ordem de inserção das tuplas no arquivo, ex: Tupla 1 : primeira inserção no arquivo,
 * Tupla 2 : segunda inserção.
 * 
 * @return int - A saída dessa função representa um status.
 * Possíveis saídas : 
 * 		FILE_NOT_FOUND : o arquivo fs_tabela.dat não foi encontrado no disco;
 * 		OUT_MEMORIA : Algumas das variáveis internas são nulas, em outras palavras não foi possíveis fazer alocação
 * 					 de memória;
 * 		TABLE_NOT_FOUND : A tabela passada pelo @param nomeTabela não existe no arquivo fs_tabela.dat;
 * 		FILE_META_NOT_FOUND : Arquvo fs_coluna.dat não foi encontrado no disco;
 *   	FILE_DATA_NOT_FOUND : O Arquivo de dados não foi encontrado no disco;
 * 		TUPLE_NOT_FOUND : o @param contador é menor que 1 ou maior que o número de tuplas no disco;
 * 		OKAY : A função copiou a tupla para o buffer e finalizou com sucesso.
 * 
 * *Lembre-se de liberar o espaço alocado para bufferPool ao final.
 **/
int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador);

//Função que mostrará o conteúdo do Buffer Pool na tela
int showBuffer(buffer *bufferPool,int page);

int getTupleNumber(FILE *arquivo, int position, int tamTuple);

/**
 *Extrai uma tupla específica do buffer, considerando a ordem de inserção. 
 * 
 * Funcionamento: O número da tupla a ser removida deve ser maior que 0 e menor
 * que o número de elementos no buffer. Após obter o número da página e a posição
 * inicial da tupla, a função aloca uma estrutura conforme os metadados corretos
 * dela. Assim é gravado e retornado ao chamador.
 * 
 * @param buffer *bufferPool - variável não pode ser nula;
 * @param int tupleNumber - Número da tupla a ser extraída. Considera a ordem de inserção no buffer.
 * @return Element_t - Após pegar uma tupla do buffer, é alocado uma struct
 * assim, ela é devolvida com seus devidos elementos.
 * Em caso de bufferPool for nulo/vazio ou tupleNumber não foi um número válido ou a função não conseguir
 * alocar memória, a mesma retorna nulo.
 * 
 **/
Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber, int *quantidade);

/**
 * Cria uma tabela em disco conforme os parâmetros passados.
 * 
 * @param char *tableName - Nome lógico da tabela a ser criada. Aceita apenas letras de [a-z] e [A-Z].
 * @param field *Attributes - Vetor que contém os atributos a serem criados. Deve-ser não nulo.
 * @param int numberAtt - Número de atributos que field *Attributes contém. A função considera apenas 
 * a quantidade passada por esse parâmetro;
 * 
 * @return int - A função devolve um flag que representação um status.
 * Possíveis saídas:
 * 		ABORT : Algum parâmetro possui um valor não permitido;
 * 		FILE_NOT_FOUND : Arquivo fs_tabela.dat não foi encontrada no disco;
 * 		TABLE_ALREADY_EXISTS : O nome da tabela passada para a função já está sendo usado;
 * 		VALOR_INVALIDO : O nome da tabela ou de algum atributo não possui apenas letras. Ou
 * 						 ainda o tipo de algum atributo é inválido. ex: fType: 'i'. é diferente de
 * 						fType: 'I'.
 *		OUT_MEMORIA : Alguma variável interna não pode ser alocada por falta de memória;
 * 		FILE_META_NOT_FOUND : Arquivo fs_coluna.dat não foi localizado;
 * 		VALUE_ALREADY_EXISTS: A tabela já existe no disco;
 * 		OKAY - A tabela e seus metadados foram devidamente criados e gravados no disco;
 * 
 * *Liberar o espaço após o uso é de responsabilidade do usuário.
 * 		
 **/
 
 Element_t *extractTuplesFromPage(buffer *bPool, int page, int *quantidade);
int createTable( char *TableName, field *Attributes, int numberAtt);

/**
 * Seu objetivo é inserir uma tupla em uma tabela específica;
 * 
 * @param char *tableName - Nome da tabela a qual a tupla deve ser inserida;
 * @param Element_T *Attributes - Vetor que contém os elementos a ser gravados.
 * Importante considerar que os dados devem estar colocados no vetor na devida
 * ordem , ou seja, conforme a ordem definida pelos metadados.
 * 
 * @return int - Retorna flag que representa um estado.
 * Possíveis saídas:
 * 		ABORT: Algum parâmetro possui um valor não permitido;
 * 		FILE_NOT_FOUND : Arquivo fs_tabela.dat não foi localizado;
 * 		FILE_DATA_NOT_FOUND : Arquivo de dados não foi encontrado;
 * 		FILE_META_NOT_FOUND : Arquivo fs_coluna.dat não foi encontrado no disco;
 * 		TABLE_NOT_FOUND : A tabela passada pelo parâmetro não existe;
 * 		VIOLATE_NUMBER_LENGHT : Para atributos do tipo int, caso houve violação do
 * 		números de dígitos definidos na criação da tabela. ex: o atributo possui fLenght : 2,
 * 		a função espera receber um número menor que 100, porém recebe um maior ou igual a 100.
 * 		OKAY : A tupla foi inserida no arquivo de dados devidamente.
 **/
int insertInto( char *tableName, Element_t *Attributes);
