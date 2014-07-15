Universidade Federal da Fronteira Sul
Ciência da Computação - Banco de Dados II
Implementação de Buffer

PRIMEIRA PARTE:
Raphael Machado dos Santos;

SEGUNDA PARTE (CACMATTHI):
Caciano Martinelli de marco,
Mateus Trebien, 
Thiago Mendes da Silva;

TERCEIRA PARTE (EDSLUC):
Edson Lemes da Silva,
Lucas Cezar Parnoff;


Partes feitas por RAPEDU e após por CACMATTHI:


FUNÇÕES:

void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount);

void findNextAvaliable(buffer *bPool);

void applyReplacementPolicies(buffer *bPool);

void bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);

int fillBuffer(buffer *bufferPool, field *fieldList,char *nomeTabela, char *arquivo);

int showBuffer(buffer *bufferPool);


TABELAS:

fs_personagem em fs_personagem.dat

fs_status em fs_status.dat

Dicionario de Dados:

dicionario.dat

Estrutura do dicionario.dat
<nome tabela>'\0'<nome tabela>.dat'\0'

Estruturas das tabelas
int(numero de colunas) praticamente igual do primeiro '.dat'
nome campo
tipo campo
tamanho campo

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    	A partir daqui são alterações feitas por EDSLUC:
Estruturas:

 --Todas as estruturas disponíveis para o usuário estão devidamente descrita no arquivo Buffer.h.
 --As estruturas que fazem parte da programas, que foram definidas no arquivo Buffer.c estão
 descritas anterior a sua declaração. 

FUNÇÕES: 

-Funções disponíveis para o usuário :
	
	1.int createTable( char *TableName, field *Attributes, int numberAtt);
	2.int insertInto( char *tableName, Element_t *Attributes);
	3.int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador);
	4.Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber);
	5.Element_t *extractTuplesFromPage(buffer *bPool, int page);
	
	1. Para criação de tabelas. Elas são criadas na memória e depois são gravados em disco.
	   Pontos importantes: Aceita a criação de tabelas cuja o nome seja apenas letras*, ou seja,
	   nomes como : 'table1' é inválido para a função.
	   
	   Acontece o mesmo para seus atributos, cuja o nome deve ser unicamente formado por letras*.
	   
	   Cada atributo possui um tipo, dentre eles, estão (I, D, S, C) que indicam int, double, string e character
	   respectivamente. Observe que quaisquer tipos diferentes, serão considerados inválidos. ex: 'i' é difente de 'I'.
	   Além disso, é importante notar que cada atributo possui um tamanho, sendo que para os tipo 'D' e 'C' o tamanho
	   é indiferente, pois os mesmos já estão definidos. Para o tipo 'S', o tamanho é considerado conforme definido
	   pelo usuário. Já para 'I' a variável que indica o tamanho, serve para organizar a quantidade de dígitos que
	   aquele atributo irá suportar. ex: Se indicamos para atributo do tipo 'I' um valor 2, então significa que ele
	   aceita valores da faixa de 0 até 99.

	   
	   
	   *Entenda-se por letra: Conjunto de 'a' até 'z' e 'A' até 'Z'.

	2. Função usada para inserir uma tupla em uma tabela existente. Insere apenas uma tupla. Caso queira inserir
	   mais de uma vez, essa função deve ser chamada n vezes.
	   POntos importante : Essa função garante que caso um atributo de uma tupla for um número inteiro, esse deve
	   respeitar o limite de dígitos definidos anteriormente.
	   Não possui nenhuma limitação referentes a string, ou seja, não grava apenas letras.

     	   Existe um arquivo main.c, nele estão descritos um exemplo de criação e inserção de tabelas.

	3. Função carrega uma tupla do arquivo no disco e trás para memória. Cada página armazena tuplas da mesma tabela,
	   ou seja, se na primeira página estão tuplas da tabela X, então ao inserir elementos da tabela Y, esses mesmo
	   serão gravados na segunda página.

      	   Na primeira inserção de cada tabela no buffer, é gravado seu metadados.
	   As constantes : BP_PAGES E BP_SIZE, controlam a quantidade de página disponíveis no buffer, e o tamanho de cada
	   uma, respectivamente. Ambas estão definidas no arquivo Buffer.h, e podem ser alteradas pelo usuário.

	   Quando o buffer enche, deve-se escolher uma página para ir para o disco, neste caso definimos a política de troca LRU.

	   Adversidades: 
	        3A .Ao escolher uma página para gravar no disco, toda ela será regravada. Em outras palavras, todas as tuplas
	   	que estão naquela página vão para o arquivo em disco;  

	        3B .Quando uma página enche, mas possui páginas livres. Está página não poderá mais ser receber novos dadas até que
	        ela vá para o disco. Essa não parte não é controlada por essa implementação. O que pode-se fazer é aumentar o tamanho
	        da página, na constante BP_sIZE.
		
	        3C .Constante acesso a disco.


	4. Extrai uma tupla do buffer. Essa função considera a ordem de inserção no buffer, ou seja, ao escolher um número da tupla,
	   a mesma é contada, e retorna caso for encontrada.
	   
	   Não difere as tabelas, retira apenas na ordem. A estrutura que retorna possui um tipo sendo, (String, Ddouble, Dint, Caracter),
	   assim, o usuário poderá saber qual o tipo da informação retornada.

	   Adversidade:

	        4A. Recebe um ponteiro para um inteiro que ao final da execução receberá a quantidade de elementos na estrutura que                			será retornada.

	5. Extrai um conjunto de tuplas existentes em uma página X. Basicamente, retorna várias tuplas de uma mesma tabela.

	   Assim como a função 4, não difere as tabelas, retira na ordem que estão organizadas na página. Retorna uma estrutura
	   semelhante, porém como mais espaços, já que essa função pode pegar mais de uma tupla. 

	   Adversidade:           
		
	        5A. Recebe um ponteiro para inteiro, e ao final recebe a quantidade de elementos da estrutura retornada.


Detalhes sobre os parâmetros e retornos dessas funções, podem ser encontrados antes de seu protótipo no arquivo Buffer.h.
**Importante ressaltar que : As estruturas alocadas pelas funções 4 e 5 devem ser liberadas pelo usuário. Assim como a estrutura
do bufferPool.
	
----------------------------------------------------
Funções NÃO disponíveis para o usuário:;


	1.int bufferInsert(buffer *bPool,char *tupla,int tupleLenght, field *fieldList, int fieldCount, int id); 
	2.int getTupleNumber(FILE *arquivo, int position, int tamTuple);
	3.int getTuplePosition(buffer *bufferPool, int tupleNumber, int *pagina);
	4.int procura_meta(buffer *bPool, int id, int tamanho);
	5.int testeTam( int numero, int count);
	6.int tupleTam( field *fieldList, int fieldCount);
	7.int verifyCaracter( char nome);
	8.int searchTable(FILE *arquivo, char *Tabela, criar **myTable);
	9.int checkMeta( field estrutura[], int numberAtt);
	10.int counter( int init, FILE *metadados, int total, criar *myTable );
	11.int returnDisk(bufferPage *bp);
	12.char *GeneratePhysName( char *last);
	13.void cpyvar(char *tupla,char *vd,int init,int tam);
	14.void copia_string( char *destino,char *origem, int initOrigem, int destTam);
	15.void initBuffer(buffer *bPool);


	1. Função que concretiza a inserção de uma tupla no buffer. Após a tupla requisitada ser encontrada e 
	   devidamente aloca, essa função é chamada e finalizado o processo de inserção.
	
	2. Auxilia na hora de contar as tuplas que estão gravadas no arquivo de dados, deste modo, a função 
      	   consegue saber a partir de qual byte deve-se ler a tupla.

	3. Procura nas páginas do buffer, a posição inicial de uma tupla passada por parâmetro.

	4. Simples comparação do identificador da tabela para saber se ela está presente no buffer ou não.

	5. Função verifica se um número respeita o limite de dígitos impostos pelas funções chamadoras.

	6. Função soma os tamanhos dos atributos. Em outras palavras, conta o tamanho total de uma tupla.

	7. Verifica se um caracter respeita o limite de : 'a' até 'z' e 'A' até 'Z'.

	8. Procura no arquivo fs_tabela.dat  a tabela passada por parâmetro.

	9. Essa função é responsável por analisar o nome e o tipo de um atributo, assim garantindo que ele seja válido.
	
	10. Conta o números de atributos de uma tabela, analisando o número que identifica a tabela. A contagem é feita
	    a partir do arquivo fs_coluna.dat, onde estão definidos os metadados.
	
	
	11. Essa função grava uma página do buffer no disco. (maiores detalhes no arquivo Buffer.c);
	
	12. Função que gera um nome físico para os arquivos de uma tabela, ex : O formato escolhido é file_0.dat,
	    substituindo 0 por um número sequêncial maior que zero.

	
	13. Copia uma sequência de caracteres de acordo com os tamanhos específicados;
	    Os dados copiados, são gravador a partir de uma posição x na String.

	14. Copia uma sequência de caracteres de acordo com os tamanhos específicados;
      	    Função semelhante a número 12, entretando nessa os dados são gravados sempre a partir do índice zero.

	15. Inicializa o buffer com os valores default.


*Os parâmetros e retornos desta função estão descritos ao início de cada função no arquivo Buffer.c.
---------------------------------------
Organização do Dicionário de dados:

 --Possui um diretório chamado 'files'. Este diretório é fixo e NÃO pode ser alterado,
assim como, não pode ser renomeado.

dentro dele estão presentes dois arquivos .dat e um diretório chamado 'data'.

Primeiro deles : fs_tabela.dat;
 -Neste arquivo estão descritos informações gerais sobre as tabelas.
 
 -Exatamente nesse formato : id (int) / nome lógico (30bytes) / nome físico (30bytes) / diretório (30bytes).
 -Onde : *id - é um número inteiro que representa a identificação da tabela. O id é contado a partir do 1.
	 *nome lógico - Nome utilizado pelo usuário para localizar a tabela.
	 
	 *nome físico - Nome definido pelo programa. Cujo formato segue : file_id.dat (onde id é o número inteiro que identifica a tabela);
	 Todas as tuplas dessa tabela serão gravadas nesse arquivo.	 
	
	 *dir - Localização em disco do arquivo de dados da tabela.
 
-A estrutura da tabela está definida no início do arquivo Buffer.c. Ela chama-se Ctabela.

Segundo deles : fs_coluna.dat;
  -Aqui serão gravados os metadados das tabelas.    

  -Formato : id (int) / nome_atributo (TNAME_LENGHT) / tipo_atributo (1byte) / tamanho_atributo( int)

  -TNAME_LENGTH : Pode ser alterado pelo usuário, e está definida em Buffer.h.

  -Onde : *id - número que identifica a tabela, deve ser o mesmo que está gravado em fs_tabela.dat.
       	  *nome_atributo: Nome que identifica o atributo com tamanho máximo de TNAME_LENGHT.
      	  *tipo_atributo: Caracter que identifica o tipo do atributo.
      	  *tamanho_atributo: Quantidade de bytes que o atributo ocupa em disco.

-A estrutura de metadados está definida no arquivo Buffer.h, seu nome : field;

Terceiro deles: diretório chamado 'data';
    dentro dele: estarão os arquivos de dados das tabelas. Estarão armazenados todos os
    arquivos que tenham o formato 'file_id.dat';

***É importante ressaltar que nenhum dos diretórios e arquivos citados aqui podem ser movidos e/ou renomeados.
   Uma vez que todos são fixos.


--------
Exemplificando a organização:

Imagine 2 tabelas e um insertInto em cada uma:
	A.-Cliente (nome varchar(10), idade int(2)); insertInto cliente( 'Primeiro', 25);
	
	B.-Produto (nome varchar(20), preco double); insertInto Produto ( 'Teclado', 20.50);

	Nos arquivos elas estão deste modo:


	files/fs_tabela.dat

	id | nome lógico | nome físico | dir
	-------------------------------------
	 1 | Cliente     | file_1.dat  | data/file_1.dat
	 2 | Produto     | file_2.dat  | data/file_2.dat


	files/fs_coluna.dat

	id | nome atributo | tipo atributo | tamanho atributo
       	-----------------------------------------------------
       	1 |      nome      |      S        | 10
      	1 |      idade     |      I        |  2
	2 |      nome      |      S        | 20
      	2 |      preco     |      D        | 8

	*Tipo I armazena tamanho 2, pois são permitidos dois digitos apenas.

	files/data/files_1.dat

	   nome    | idade
	---------------
	  Primeiro | 25

	fies/data/files_2.dat :

	  nome  | preco
	----------------
	Teclado | 20.50000

	*Obviamente que os nomes de índices dessas tabelas não estão gravados nos arquivos, é apenas para representação neste exemplo.


----------------------
Este programa foi compilado com o GCC da seguinte maneira:
	:gcc *.c -o main -Wall 
