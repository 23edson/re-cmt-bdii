

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BP_PAGES 16
#define BP_SIZE 512

typedef struct field{
    char fName[20];
    char fType;
    int fLenght;
}field;

/*typedef struct field_elem{
	int id;
	field *fieldList;
	int fieldCount;
}felem;
*/
typedef struct bufferPage
{
	int idNumber;
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

void cpyvar(char *tupla,char *vd,int init,int tam){
		//copiar valores tipo double e int para a string
		
		int i=init,j=0;
		for (;i<tam+init;i++)
			tupla[i]=vd[j++];
}

void initBuffer(buffer *bPool){
	int i;
	//Inicializa o buffer com todos os diskSeek em -1 para ser a flag que verifica se está em uso ou não.
	for(i = 0; i < BP_PAGES; i++){
		bPool->bp[i].diskSeek = 0;
		bPool->bp[i].data = malloc(sizeof(char)*BP_SIZE);
		bPool->bp[i].fieldList = NULL;
		bPool->bp[i].fieldCount = 0;
	}
	bPool->countItems = 0;
	bPool->nextPageAvaliable = 0;
	//bPool->itens  = malloc(sizeof(bufferPage));
	
	//bPool->itens->fieldCount = 2;
	//bPool->itens->id = 1;
	//bPool->diskSeek = 0;
	//bPool->countMeta = 1;
	//bPool->fieldList = fieldList;
	//bPool->fieldCount = fieldCount;
}
int procura_meta(buffer *bPool, int id, int tamanho){
	int i;
	
	for( i = 0; i< tamanho; i++){
		if(bPool->bp[i].idNumber == id)
			return i;
	}
	return -1;
}
int bufferInsert(buffer *bPool,char *tupla, int diskSeek, int tupleLenght,field *fieldList, int id, int fieldCount){
	
	if(!bPool)
		puts("nao");
	
	//int i = 0;
	//Primeiro verifica se o buffer está cheio, caso esteja ele aplica as políticas de troca
	/*if(bPool->countItems >= BP_PAGES){
		applyReplacementPolicies(bPool);//puts("aquixxxxxxxxxxxxxxxxx");
	}*/
	//É inserido na próxima página disponível
	
	int teste = procura_meta(bPool, id, bPool->countMeta);
	
	if(teste < 0){
		bPool->bp[bPool->nextPageAvaliable].fieldList = fieldList;
		bPool->bp[bPool->nextPageAvaliable].fieldCount = fieldCount;
	    bPool->bp[bPool->nextPageAvaliable].idNumber = id;
		teste = bPool -> nextPageAvaliable;
		bPool->countMeta++;
	}
	
	if( BP_SIZE - bPool->bp[teste].diskSeek > tupleLenght)
		cpyvar(bPool->bp[teste].data, tupla, bPool->bp[teste].diskSeek, tupleLenght);
			
		bPool->bp[teste].diskSeek += tupleLenght;
	
	
	
	//strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
	bPool->bp[teste].pinCount = 0;
	bPool->bp[teste].rewriteBit = 0;
	bPool->nextPageAvaliable++;
	bPool->countItems ++;
	//É procurado a próxima página disponível
	//findNextAvaliable(bPool);
	return 0;
}
int main(){

	char Tuple[16];
	char Tuple1[4];
	char Tuple2[8];
	char Tuple3[8];
	int i = 1;
	
	Tuple[0] = '1';
	Tuple[1] = 'C';
	Tuple[2] = 'o';
	Tuple[3] = 'r';
	Tuple[4] = 's';
	Tuple[5] = 'a';
	Tuple[6] = '\0';
	Tuple[7] = 'c';
	Tuple[8] = 'c';
	Tuple[9] = 'c';
	Tuple[10] = 'c';
	Tuple[11] = 'c';
	Tuple[12] = 'c';
	Tuple[13] = 'c';
	Tuple[14]= 'c';
	Tuple[15] = 'A';
	
	Tuple1[0] = '1';
	Tuple1[1] = '2';
	Tuple1[2] = '3';
	Tuple1[3] = '4';
	
	buffer *buff = malloc(sizeof(buffer));
	field *att = NULL;
	field *att1 = NULL;
	//initBuffer(buff);
	att = malloc(sizeof(field)*2);
	att1 = malloc(sizeof(field) * 3);
	strcpy(att[0].fName, "Nome");
	att[0].fType = 'S';
	att[0].fLenght = 15;
	
	strcpy(att[1].fName, "Letra");
	att[1].fType = 'C';
	att[1].fLenght = 1;puts("aq");
	initBuffer(buff);
	
	
	strcpy(att1[0].fName,"Nome");
	strcpy(att1[1].fName, "letra");
	strcpy(att1[2].fName, "otro");
	
	att1[0].fType = 'S';
	att1[1].fType = 'C';
	att1[2].fType = 'C';
	
	att1[0].fLenght = 5;
	att1[1].fLenght = 1;
	att1[2].fLenght = 1;
	
	Tuple2[0] = '2';
	Tuple2[1] = 'A';
	Tuple2[2] = 'B';
	Tuple2[3] = 'C';
	Tuple2[4] = '\0';
	Tuple2[5] = 'v';
	Tuple2[6] = 'D';
	Tuple2[7] = 'E';
	
	Tuple3[0] = '2';
	Tuple3[1] ='H';
	Tuple3[2] ='K';
	Tuple3[3] = '\0';
	Tuple3[4] = 'z';
	Tuple3[5] = 'z';
	Tuple3[6] = 'X';
	Tuple3[7] = 'Z';
	
	//(buffer *bPool,char *tupla, int diskSeek, int tupleLenght,field *fieldList, int id, int fieldCount)
	bufferInsert(buff,Tuple,0,16,att,1,2);
	
	printf("%c", buff->bp[0].data[15]);
	//bufferInsert(buff,Tuple1,0,4,att);
	bufferInsert(buff,Tuple2,0,8,att1,2,3);
	bufferInsert(buff,Tuple3,0,8,att1,2,3);
	printf(" %c", buff->bp[1].data[1]);
	printf(" %c ", buff->bp[1].data[9]);
	//FILE *arq = fopen("dicionario.dat", "a+");
	
	return 0;
}
