//Este trabalho utiliza a licença GNU General Public License.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "erro.h"

#define TNAME_LENGHT 20
#define BP_PAGES 16
#define BP_SIZE 512

typedef struct field{
    char fName[TNAME_LENGHT];
    char fType;
    int fLenght;
}field;

typedef struct bufferPage
{
	int idNumber;
	int countTuples;
	field *fieldList;
	int fieldCount;
	int diskSeek; //Posição no disco para poder regravar os dados se necessário
	int pinCount; //Pin Count
	int rewriteBit;//Dirty Bit
	char *data; //Tupla de dados
}bufferPage;
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
int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador);

//Função que mostrará o conteúdo do Buffer Pool na tela
int showBuffer(buffer *bufferPool,int page);

int getTupleNumber(FILE *arquivo, int position, int tamTuple);

Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber);

int createTable( char *TableName, field *Attributes, int numberAtt);
int insertInto( char *tableName, Element_t *Attributes);
