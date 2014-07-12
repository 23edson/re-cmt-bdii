//Este trabalho utiliza a licença GNU General Public License.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "erro.h"

#define TNAME_LENGHT 20
#define BUFFER_SIZE 16

typedef struct field{
    char fName[TNAME_LENGHT];
    char fType;
    int fLenght;
}field;

typedef struct bufferPage
{
	int diskSeek; //Posição no disco para poder regravar os dados se necessário
	int pinCount; //Pin Count
	int rewriteBit;//Dirty Bit
	char *data; //Tupla de dados
}bufferPage;
typedef struct buffer
{
	int countItems;
	int nextPageAvaliable;
	field *fieldList;
	int fieldCount;
	bufferPage bp[BUFFER_SIZE];
}buffer;

/*typedef struct tupla{
	char tipo;
	char *String;
	int *Dint;
	double *Ddouble;
}myRow;*/
typedef struct Elemento {
  enum ElementType { String, Nint, Ndouble,Caracter }type;
  union {
    char      *Str;
    int       *Dint;
    double    *Ddouble;
  };
}Element_t;

//Inicializa o buffer
void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount);

//Procura a próxima página disponível
void findNextAvaliable(buffer *bPool);

//Aplica as políticas de troca
void applyReplacementPolicies(buffer *bPool);

//Insere um novo elemento no buffer
int bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);

//Função que vai ler os arquivos de dados e metadados
int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador);

//Função que mostrará o conteúdo do Buffer Pool na tela
//int showBuffer(buffer *bufferPool,int page); função grupo anterior

// retorna posição da tupla que está procurando no arquivo, ou TUPLE_NOT_FOUND
int getTupleNumber(FILE *arquivo, int position, int tamTuple);

//Função que retorna uma estrutura para o usuario imprimir os dados
Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber);

//função para criar uma tabela com (nome da tabela, atributos ver 'struct field' acima, numero de atributos)
//em caso da tabela ja ter sido criada retorna OKAY 
//outros retornos OUT_MEMORIA / VALOR_INVALIDO / ABORT / FILE_NOT_FOUND
int createTable( char *TableName, field *Attributes, int numberAtt);

//função para inserir uma tupla no arquivo de dados (nome da tabela, atributos ver 'struct Element_t' acima)
//em caso da tabela ja ter sido criada retorna OKAY / OUT_MEMORIA / TABLE_NOT_FOUND / ABORT / FILE_NOT_FOUND
// VIOLATE_NUMBER_LENGTH
int insertInto( char *tableName, Element_t *Attributes);
