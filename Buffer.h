//Este trabalho utiliza a licença GNU General Public License.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TNAME_LENGHT 20
#define BUFFER_SIZE 16
#define BUFFER_PREENCHIDO 0
#define ERRO_ARQUIVO 1
#define TABELA_NOTFOUND 2
#define BUFFER_VAZIO 3

typedef struct field
{
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

//Inicializa o buffer
void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount);
//Procura a próxima página disponível
void findNextAvaliable(buffer *bPool);
//Aplica as políticas de troca
void applyReplacementPolicies(buffer *bPool);
//Insere um novo elemento no buffer
void bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);
//Função que vai ler os arquivos de dados e metadados
int fillBuffer(buffer *bufferPool, field *fieldList,char *nomeTabela, char *arquivo);
//Função que mostrará o conteúdo do Buffer Pool na tela
int showBuffer(buffer *bufferPool);