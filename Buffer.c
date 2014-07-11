
//Este trabalho utiliza a licença GNU General Public License.

#include "Buffer.h"

/*
 * 
 * @defines constantes
 **/  
#define CONST 30
#define BASE 10
#define CONST_MAX 94  //Constantes que nao interessam ao usuário
#define CONST_META 29



#define FILE_NOT_FOUND -5 //provisório ( esses valores estarão no arquivo error.h)
#define VALUE_ALREADY_EXISTS -6
#define VALOR_INVALIDO -7
#define OKAY -1 
#define ABORT -8
#define ERRO_ATT_NUMBER -9
#define VIOLATE_NUMBER_LENGTH -10
#define TABLE_ALREADY_EXISTS -11
#define TABLE_NOT_FOUND -12
#define FILE_META_NOT_FOUND -13
#define FILE_DATA_NOT_FOUND -14
#define TUPLE_NOT_FOUND -16

enum ElementType { String, Nint, Ndouble,Caracter };
typedef struct Element {
  enum ElementType type;
  union {
    char      *Str;
    int       *Dint;
    double    *Ddouble;
  };
}Element_t;



struct Ctabela{
		int id;
		char lnome[CONST]; //Estrutura de fs_tabela.dat. De acordo com a parte anterior do trabalho
		char fnome[CONST];
		char dir[CONST];
};
typedef struct Ctabela criar;

union c_int{
		int  num;
		char cnum[sizeof(int)];
	};//union para o tipo inteiro

	union c_double{
		double numd;
		char   cnumd[sizeof(double)];
	};
	

void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount)
{
	int i;
	//Inicializa o buffer com todos os diskSeek em -1 para ser a flag que verifica se está em uso ou não.
	for(i = 0; i < lenght; i++)
	{
		bPool->bp[i].diskSeek = -1;
	}
	bPool->countItems = 0;
	bPool->nextPageAvaliable = 0;
	bPool->fieldList = fieldList;
	bPool->fieldCount = fieldCount;
}
void cpyvar(char *tupla,char *vd,int init,int tam){
		//copiar valores tipo double e int para a string
		
		int i=init,j=0;
		for (;i<tam+init;i++)
			tupla[i]=vd[j++];
}
int extractTupleFromBP(buffer *bufferPool ,int tupleNumber){
	int i;
	//struct Element *arr =  * 3);
	Element_t *myTuple = NULL;
	int sum = 0;
	
	if(tupleNumber < 1 || tupleNumber > bufferPool->countItems)
		return TUPLE_NOT_FOUND;
	
	tupleNumber--;
	//printf("Bu; %d,%d,%d,%d", bufferPool->nextPageAvaliable,bufferPool->countItems,bufferPool->fieldCount, bufferPool->bp[0].diskSeek);;
	myTuple = (Element_t *)malloc(sizeof(Element_t) * bufferPool->fieldCount);
	if(!myTuple){
		return OUT_MEMORIA;
	}
	
	for(i = 0; i < bufferPool->fieldCount; i ++)
	{
		if(bufferPool->fieldList[i].fType == 'S'){
			myTuple[i].type = String;
			myTuple[i].Str = (char *) malloc(sizeof(char)*bufferPool->fieldList[i].fLenght);
			cpyvar(myTuple[i].Str, bufferPool->bp[tupleNumber].data, sum, bufferPool->fieldList[i].fLenght);
			sum += bufferPool->fieldList[i].fLenght;
		}
		else if(bufferPool->fieldList[i].fType == 'I'){
			myTuple[i].type = Nint;
			//myTuple[i].Dint = (int *)malloc(sizeof(int));
			
			myTuple[i].Dint = (int *)&bufferPool->bp[tupleNumber].data[sum];
			sum += sizeof(int);
		}
		else if(bufferPool->fieldList[i].fType == 'D'){
			//myTuple[i].Ddouble = (double *)malloc(sizeof(double));
			myTuple[i].type = Ndouble;
			myTuple[i].Ddouble = (double *)&bufferPool->bp[tupleNumber].data[sum];
			sum += sizeof(double);
		}
		else{
			//myTuple[i].String = (char *)malloc(sizeof(char));
			myTuple[i].type = Caracter;
			myTuple[i].Str = malloc(sizeof(char));
			*myTuple[i].Str =bufferPool->bp[tupleNumber].data[sum];puts("aquooi");
			sum += sizeof(char);
		}	
		//myTuple[i].tipo = bufferPool->fieldList[i].fType;
		 //printf("%s",bufferPool->fieldList[i].fName);
		
	}
	printf("\n");
	puts(myTuple[0].Str);
	printf("double: %f", *myTuple[1].Ddouble);
	printf("int : %d", *myTuple[2].Dint);
	printf("caracter: %c", *myTuple[3].Str);
	/*printf("%c", bufferPool->bp[0].data[0]);
	printf("%c", bufferPool->bp[0].data[1]);
	printf("%c", bufferPool->bp[0].data[2]);
	printf("%c", bufferPool->bp[0].data[3]);
	printf("%c", bufferPool->bp[0].data[4]);
	printf("%c", bufferPool->bp[0].data[15]);
	
	double *l = (double *)&bufferPool->bp[0].data[16];
		printf("%f", *l);
		printf("%c", bufferPool->bp[0].data[24]);
		int *d = (int *)&bufferPool->bp[0].data[25];
		printf("V: %d", *d);
		printf("%c", bufferPool->bp[0].data[29]);
		printf("%c", bufferPool->bp[0].data[30]);
		printf("%c", bufferPool->bp[0].data[31]);
	*/
	return 0;
}
void findNextAvaliable(buffer *bPool){
	int i;
	//Simples verificação se a página está disponível se do diskSeek for igual à -1
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(bPool->bp[i].diskSeek == -1)
		{
			bPool->nextPageAvaliable = i;
			break;
		}

	}
}

void applyReplacementPolicies(buffer *bPool)
{
	//Aqui é escolhido a página do buffer que tem o menor pinCount para ser substituida
	int i, lower = bPool->bp[0].pinCount;
	bufferPage bPage = bPool->bp[0];
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(bPool->bp[i].pinCount < lower)
		{
			bPage = bPool->bp[i];
			lower = bPool->bp[i].pinCount;
		}
	}
	//A página encontrada fica armazenada em bPage
	bPage.rewriteBit = 1;
	if(bPage.rewriteBit == 0)
	{
		bPool->bp[i].diskSeek = -1;
		bPool->nextPageAvaliable = i;
	}
	else
	{
		//Aqui deve ser colocado o código para regravação do arquivo.

	}
}
int bufferInsert(buffer *bPool,char *tupla, int diskSeek, int tupleLenght){
	
	if(!bPool)
		//puts("nao");
	//Primeiro verifica se o buffer está cheio, caso esteja ele aplica as políticas de troca
	if(bPool->countItems >= BUFFER_SIZE)
	{
		applyReplacementPolicies(bPool);//puts("aquixxxxxxxxxxxxxxxxx");
	}
	//É inserido na próxima página disponível
	bPool->bp[bPool->nextPageAvaliable].diskSeek = diskSeek;
	if((bPool->bp[bPool->nextPageAvaliable].data = (char *)malloc(tupleLenght*sizeof(char)))==NULL){
		return OUT_MEMORIA;
	};
	cpyvar(bPool->bp[bPool->nextPageAvaliable].data, tupla, 0, tupleLenght);
	//strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
	bPool->bp[bPool->nextPageAvaliable].pinCount = 0;
	bPool->bp[bPool->nextPageAvaliable].rewriteBit = 0;
	bPool->countItems ++;
	//É procurado a próxima página disponível
	findNextAvaliable(bPool);
	return 0;
};
int showBuffer(buffer *bufferPool,int page)
{
	if (!bufferPool) return BUFFER_VAZIO;
	if(page>=BUFFER_SIZE) return INVALID_PAGE;
	if(!bufferPool->countItems) return BUFFER_VAZIO;
	//Primeiro é mostrado os campos
	int i, j=0;
	printf("\n");
	for(i = 0; i < bufferPool->fieldCount; i ++)
	{
		printf("%s",bufferPool->fieldList[i].fName);
		if (i==0) printf("\t");  
		printf("\t");
	}
	printf("\n");
	
	//Depois é mostrado o conteúdo do buffer
	//Só é mostrado a página que esta na referencia.
	if(bufferPool->bp[page].diskSeek != -1) {
		j = 0;
		while(bufferPool->bp[page].data[j] != '\0') {
			if (bufferPool->bp[page].data[j] == '#') printf("\n"); // fim da tupla
			else {
				if (bufferPool->bp[page].data[j] == '*') printf("\t");
				else printf("%c", bufferPool->bp[page].data[j]);
			}	
			j++;		
		}
	}
	printf("\n\n");
	return 0;
}
int counter( int init, FILE *metadados, int total, criar *myTable ){
	//Essas Função conta o número de atributos de uma tabela
	int copiar = myTable->id;
	int AttCount = 0;
	int i = init; //
	
	fseek( metadados, init , SEEK_SET);
	//printf("aqui : %d, %d, %d, %ld", i,copiar,total,ftell(metadados));
	while(i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);//printf("cop: %d  e %ld", copiar, ftell(metadados));
		if(myTable->id != copiar)
			break;
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
		
	}//puts("ccccccccccccccc");
	//printf("aqui : %d", AttCount);
	return AttCount;
}

int getTupleNumber(FILE *arquivo, int position, int tamTuple){
	if(position < 1)
		return TUPLE_NOT_FOUND;
	
	int analise = 0;
	int endLoop = 0;
	fseek(arquivo, 0, SEEK_END);
	long tam = ftell(arquivo);
	rewind(arquivo);
	//printf("Tam: %ld, %d", tam, tamTuple);
	while(endLoop < tam){
		fseek(arquivo, tamTuple, SEEK_CUR);
		analise ++;
		if(position == analise){
			fseek(arquivo, -tamTuple, SEEK_CUR);//printf("ss: %ld, %d", ftell(arquivo), position);
			return ftell(arquivo);
		}
		endLoop += tamTuple;
	}
	return TUPLE_NOT_FOUND;
	
}

int fillBuffer(buffer *bufferPool, char *nomeTabela, int contador){
	
	FILE *tabela = fopen("files/fs_tabela.dat","r");
	if(tabela == NULL) return FILE_NOT_FOUND;
	
	
	//Abertura dos arquivos de dados e de metadados
	char *name = NULL; //Name representa o nome da tabela
	if((name=(char*)malloc(50))==NULL){
		fclose(tabela);
		return OUT_MEMORIA;
	}
	
	char *caminho=NULL; 
	if((caminho=(char*)malloc(50))==NULL){
		fclose(tabela);
		free(name);
		return OUT_MEMORIA;
	}
	
	criar *biblio=NULL;
	biblio=(criar*)malloc(sizeof(criar));
	if(biblio==NULL){
		fclose(tabela);
		free(name);
		free(caminho);
		return OUT_MEMORIA;
	}
	
	int i = 0, achou=0;
	int endLoop = 0;
	
	fseek(tabela,0,SEEK_END); //Coloca o ponteiro do arquivo (tabela) no final.
	long total = ftell(tabela); //Retorna o posição atual do ponteiro no arquivo.
	rewind(tabela); //Coloca o ponteiro no início do arquivo.
	
	for(;endLoop < total; endLoop += CONST_MAX ){
		fread(&biblio->id, sizeof(int), 1, tabela);
		fread(biblio->lnome,sizeof(char),CONST,tabela);
		fread(biblio->fnome,sizeof(char),CONST,tabela);
		fread(biblio->dir,sizeof(char),CONST,tabela);
		if((strcmp(biblio->lnome,nomeTabela))==0){	
			achou=1;
			strcpy(caminho,biblio->dir);
			strcat(caminho,biblio->fnome);
			break; // sai do loop pois encontrou a tabela
		}
		
	}
	if (achou == 0){ 
		fclose(tabela);
		free(name);
		free(caminho);
		free(biblio);
		return TABLE_NOT_FOUND;
		 // fecha arquivo do dicionario
	}
	fclose(tabela);
	
	FILE *meta=fopen("files/fs_coluna.dat","r");// abre meta-dados
	if(meta == NULL){ 
		 
		free(name);
		free(caminho);
		free(biblio);
		return FILE_META_NOT_FOUND;
	}
	FILE *arquivo = fopen(caminho,"r"); // abre arquivo de dados
	if(arquivo == NULL){ 
		 
		fclose(meta);
		free(name);
		free(caminho);
		free(biblio);
		return FILE_DATA_NOT_FOUND;
	}
	
	int copiar = 0;
	
	fseek(meta,0,SEEK_END);
	total = ftell(meta);
	long pos = -1;
	rewind(meta); 
	
	 
	while(i < total){ //Procura a posicao inicial do metadados
		
		fread( &copiar, sizeof(int), 1, meta);
		if(copiar == biblio->id){
			pos = ftell(meta) - sizeof(int);
			break;
		}
			
		else{
			fseek( meta, CONST_META - sizeof(int) , SEEK_CUR);
			i +=CONST_META;
		}
	}
	
	if( pos == -1){
		 
		fclose(meta);
		fclose(arquivo);
		free(name);
		free(caminho);
		free(biblio);
		return TABLE_NOT_FOUND; //Table Not Found
	}
	//printf("posss : %ld", pos); 
	int fieldCount = counter( pos, meta, total, biblio); //Números de atributos da tabela
	
	fseek(meta, pos, SEEK_SET); //Seta o ponteiro para o início dos atributos da tabela 

	
	field *fieldList = NULL;
	if(( fieldList = malloc(sizeof(field) * fieldCount))==NULL){
			 
			fclose(meta);
			fclose(arquivo);
			free(name);
			free(caminho);
			free(biblio);
			return OUT_MEMORIA;
	}; // aloca lista com o numero de campos que tem a tabela

	
	if(!bufferPool){ //recebe ponteiro null para saber quando deve ser inicializado
		bufferPool = (buffer *)malloc(sizeof(buffer));
		if(!bufferPool){
			fclose(meta);
			fclose(arquivo); 
			free(name);
			free(caminho);
			free(biblio);
			free(fieldList);
			return OUT_MEMORIA;
		}
		initBuffer(bufferPool, BUFFER_SIZE , fieldList, fieldCount);
	}	
	//int achou = 0;
	i = 0;
	//int j,breakPoint = 0;
	int tupleLenght = 0;
	
	
    
	for(i = 0; i < fieldCount; i++){
		fseek( meta, sizeof(int), SEEK_CUR);
		fread(&fieldList[i].fName,sizeof(char),TNAME_LENGHT, meta);
		fread(&fieldList[i].fType,sizeof(char),1,meta);
		fread(&fieldList[i].fLenght,sizeof(int),1,meta); //printf("vai:%d", fieldList[i].fLenght);
		//Vai montando o tamanho da tupla com base nos tamanhos dos campos encontrados
		if(fieldList[i].fType == 'D')
			tupleLenght += sizeof(double);
		else if(fieldList[i].fType == 'I') 
			tupleLenght += sizeof(int);
			
		else if(fieldList[i].fType == 'C')
			tupleLenght += sizeof(char);
		else
			tupleLenght += fieldList[i].fLenght;
	}
	
	
	//printf("%d",tupleLenght);
	contador = getTupleNumber(arquivo,contador,tupleLenght);
	if(contador == TUPLE_NOT_FOUND)
		return contador;
	//até aqui vamos deixar no arquivo colunas.dat
	//Cria os campos temporários para a montagem da tupla
	fclose(meta);
	int *tInt = NULL;
	if((tInt=malloc(sizeof(int)))==NULL){
		 
		fclose(arquivo);
		free(name);
		free(caminho);
		free(biblio);
		free(fieldList);
		
		return OUT_MEMORIA;
	};
	double *tDouble = NULL;
	if((tDouble=malloc(sizeof(double)))==NULL){
		 
		fclose(arquivo);
		free(name);
		free(caminho);
		free(biblio);
		free(tInt);
		return OUT_MEMORIA;
	};
	char *tChar = NULL;
	char *tTuple = NULL;
	if((tTuple=malloc(sizeof(char)*tupleLenght))==NULL){
		 
		fclose(arquivo);
		free(name);
		free(caminho);
		free(biblio);
		free(tInt);
		free(tDouble);
		return OUT_MEMORIA;
	};
	
	tTuple[0] = '\0';
	union c_double vdouble;
	union c_int vint;
	
	
	/*fseek(arquivo, 0, 1);
    if(fgetc (arquivo) != EOF){
        fseek(arquivo, -1, 1);
        fread(tTuple, sizeof(char), 28, arquivo); //Traz a tupla inteira do arquivo
    }
    double *v =	(double *)&tTuple[23];
    int *n = (int *)&tTuple[23];
    char ty = tTuple[27];;
	printf("V: %f e %d, %c ", *v, *n, ty); 
	printf("%c", tTuple[0]);
	printf("%c", tTuple[1]);
	printf("%c", tTuple[2]);
	printf("%c", tTuple[3]);
	printf("%c", tTuple[4]);
	printf("%c", tTuple[5]);
	printf("%c", tTuple[6]);
	return 0;*/
	//int cont = 0;
	int thePointer = 0;
//	char caracter;
	fseek(arquivo, contador, SEEK_CUR);return 0;
	//char caracter;printf("inicio:%d", contador);
	
	fseek(arquivo, contador, SEEK_SET);
	//Começa a leitura dos dados.
	
		//Cada tupla lida é inserida no buffer
		for(i = 0; i < fieldCount; i++)
		{
			//Cada campo de tupla é checado seu tipo e adicionado corretamente na tupla temporária *tTuple
			if(fieldList[i].fType == 'I')
			{
				//Inteiro
				fread(tInt,sizeof(int),1,arquivo);
				vint.num = *tInt;
				cpyvar(tTuple,vint.cnum,thePointer,sizeof(int));
				thePointer += sizeof(int);
				/*if (i != (fieldCount-1))
					tTuple[thePointer++] = '*';
					
				 //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] = '#';
				//se acabo a tupla.
				
				//tTuple[thePointer++] = '\0';*/
				
				
			}
			else if(fieldList[i].fType == 'D')
			{
				//Double
				fread(tDouble,sizeof(double),1,arquivo); 
				vdouble.numd = *tDouble; 
				cpyvar(tTuple,vdouble.cnumd,thePointer,sizeof(double));
				thePointer += sizeof(double);
				/*if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] =  '#'; //se acabo a tupla.*/
			
				//tTuple[thePointer++] = '\0';
			}
			else if(fieldList[i].fType == 'S')
			{
				
				//String
				if((tChar = (char *)malloc(sizeof(char) * fieldList[i].fLenght))==NULL){
					 
					fclose(arquivo);
					free(name);
					free(caminho);
					free(biblio);
					return OUT_MEMORIA;
				}; 
				fread(tChar,sizeof(char),fieldList[i].fLenght,arquivo);
				
				cpyvar( tTuple, tChar, thePointer, fieldList[i].fLenght);
				thePointer =+ fieldList[i].fLenght;
				/*if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] = '#'; //se acabo a tupla.*/
				free(tChar);
				
			}
			else if(fieldList[i].fType == 'C')
			{
				//Caracter
				char c;
				fread(&c,sizeof(char),1,arquivo);
				//tChar[1] = '\0'; //Trata caracter como string
				
					tTuple[thePointer++] = c;
					
				/*if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] ='#'; //se acabo a tupla.*/
				
			
			}
		}
		/*puts("vvvvvvvvvv");
		 * 
		*/
		/*printf("%c", tTuple[0]);
		printf("%c", tTuple[1]);
		printf("%c", tTuple[2]);
		printf("%c", tTuple[3]);
		printf("%c", tTuple[4]);
		//printf("%c", tTuple[5]);
		//printf("%c", tTuple[15]);
		double *l = (double *)&tTuple[15];
		printf("%f", *l);
		//printf("%c", tTuple[24]);
		int *d = (int *)&tTuple[23];
		printf("V: %d", *d);
		printf("%c", tTuple[29]);
		printf("%c", tTuple[30]);
		printf("%c", tTuple[31]);
		fclose(arquivo); return 0;*/
	/*	//Se o arquivo de dados chegar ao fim, as tuplas param de ser entregues ao bufferPool
		if(feof(arq)) break;
		i = 0;*/
		if(bufferInsert(bufferPool,tTuple,ftell(arquivo)-tupleLenght,tupleLenght)==OUT_MEMORIA){
			 
			fclose(arquivo);
			free(name);
			free(caminho);
			free(biblio);
			free(tInt);
			//free(tChar);
			free(tDouble);
			free(tTuple);
			return OUT_MEMORIA;
		}
	//Fecha o arquivo de dados
	
	fclose(arquivo);
	free(name);
	free(caminho);
	free(biblio);
	free(tInt);
	//free(tChar);
	free(tDouble);
	free(tTuple);
	return BUFFER_PREENCHIDO;

}
 
