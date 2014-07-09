
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
#define OUT_MEMORIA -15
#define TUPLE_NOT_FOUND -16

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
void findNextAvaliable(buffer *bPool)
{
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
void cpyvar(char *tuple,char *vd,int init,int tam){
		//copiar valores tipo double e int para a string
		
		int i=init,j=0;
		for (;i<tam+init;i++)
			tuple[i]=vd[j++];
			
			
		
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
int bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght)
{
	//Primeiro verifica se o buffer está cheio, caso esteja ele aplica as políticas de troca
	if(bPool->countItems >= BUFFER_SIZE)
	{
		applyReplacementPolicies(bPool);
	}
	//É inserido na próxima página disponível
	bPool->bp[bPool->nextPageAvaliable].diskSeek = diskSeek;
	if((bPool->bp[bPool->nextPageAvaliable].data = (char *)malloc(tupleLenght*sizeof(char)))==NULL){
		return OUT_MEMORIA;
	};
<<<<<<< HEAD
	strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
=======
	cpyvar(bPool->bp[bPool->nextPageAvaliable].data, tuple, 0, tupleLenght);
	//strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
	bPool->bp[bPool->nextPageAvaliable].pinCount = 0;
	bPool->bp[bPool->nextPageAvaliable].rewriteBit = 0;
	bPool->countItems ++;
	//É procurado a próxima página disponível
	findNextAvaliable(bPool);
	return 0;
};
int showBuffer(buffer *bufferPool)
{
	if (!bufferPool->countItems) return BUFFER_VAZIO;
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
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		//Só é mostrado as páginas que estão em uso.
		if(bufferPool->bp[i].diskSeek != -1) {
			j = 0;
			while(bufferPool->bp[i].data[j] != '\0') {
				if (bufferPool->bp[i].data[j] == '#') printf("\n"); // fim da tupla
				else {
					if (bufferPool->bp[i].data[j] == '*') printf("\t");
					else printf("%c", bufferPool->bp[i].data[j]);
				}	
				j++;		
			}
		}
	}
	printf("\n\n");
	return 0;
}
int counter( int init, FILE *metadados, int total, criar *myTable ){
	//Essas Função conta o número de atributos de uma tabela
	int copiar = myTable->id;
	int AttCount = 0;
	int i = init; //printf("aqui : %d, %d, %d", i,copiar,total);
	
	fseek( metadados, init , SEEK_SET);
	
	while( copiar == myTable->id && i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);//printf("cop: %d  e %ld", copiar, ftell(metadados));
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
	}
	
	return AttCount;
}

int getTupleNumber(FILE *arquivo, int position, int tamTuple){
	
	int analise = 0;
	int endLoop = 0;
	fseek(arquivo, 0, SEEK_END);
	long tam = ftell(arquivo);
	rewind(arquivo);
	
	while(endLoop < tam){
		fseek(arquivo, tamTuple, SEEK_CUR);
		analise ++;
		if(position == analise){
			fseek(arquivo, -tamTuple, SEEK_CUR);
			return ftell(arquivo);
		}
		endLoop += tamTuple;
	}
	return TUPLE_NOT_FOUND;
	
}

int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador){
	
	FILE *tabela = fopen("files/fs_tabela.dat","r");
	if(tabela == NULL) return FILE_NOT_FOUND;
	
	
	//Abertura dos arquivos de dados e de metadados
<<<<<<< HEAD
	char *name = NULL;
	if((name=(char*)malloc(50))==NULL){
		return OUT_MEMORIA;
	};
	char *caminho=NULL; 
	if((caminho=(char*)malloc(50))==NULL){
		return OUT_MEMORIA;
	};
	criar *biblio=NULL;
	biblio=(criar*)malloc(sizeof(criar));
	if(biblio==NULL)
		return OUT_MEMORIA;
	char caractere;
	int i, a,achou=0;
	FILE *fp=NULL;
	
	if((fp=fopen(arquivo,"r"))== NULL)
		return ERRO_ARQUIVO;
	do {		
		//cuidado loop infinito
		fread(&biblio->id,sizeof(int),1,fp);
		fread(biblio->lnome,sizeof(char),CONST,fp);
		fread(biblio->fnome,sizeof(char),CONST,fp);
		fread(biblio->dir,sizeof(char),CONST,fp);
		for(i=0,a=0;i<CONST;i++){
			if(biblio->lnome[i]!='\0'){
				name[a]=biblio->lnome[i];
				a++;
			};
		};
		if((strcmp(name,nomeTabela))==0){	
			achou=1;
			caminho=biblio->dir;
=======
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
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
			strcat(caminho,biblio->fnome);
			break; // sai do loop pois encontrou a tabela
		}
		else{
			memset(name,0,strlen(name));
			free(biblio);
			if(feof(fp)==0)
				break;
		};
		
<<<<<<< HEAD
	}while(feof(fp)==0);
	if (achou == 0) return TABELA_NOTFOUND;
	fclose(fp); // fecha arquivo do dicionario
	FILE *arqm=fopen("files/fs_coluna.dat","r");// abre meta-dados
	if(arqm == NULL) return ERRO_ARQUIVO;
	
	FILE *arq = fopen(caminho,"r"); // abre arquivo de dados
	if(arq == NULL) return ERRO_ARQUIVO;
	
	int fieldCount = 0;
	
	//Lê a quantidade de campos no arquivo de metadados
	fread(&fieldCount,sizeof(int),1,arqm); // fieldcount -> numero de campos da tabela
	if((fieldList = malloc(sizeof(field) * fieldCount))==NULL){
			return OUT_MEMORIA;
	}; // aloca lista com o numero de campos que tem a tabela
	
	initBuffer(bufferPool,BUFFER_SIZE,fieldList,fieldCount);
	i = 0;
	int j,breakPoint = 0;
	int tupleLenght = 0;
	for(i = 0; i < fieldCount; i++)
	{
		for(j = 0;breakPoint == 0; j++)
		{
			//Encontra o nome do campo
			fread(&caractere,sizeof(char),1,arqm);
			if (caractere != '\0') fieldList[i].fName[j] = caractere;
			else breakPoint = j;
		}
		breakPoint = 0;
		fread(&fieldList[i].fType,sizeof(char),1,arqm);
		fread(&fieldList[i].fLenght,sizeof(int),1,arqm);
=======
	}
	if (achou == 0){ 
		fclose(tabela);
		free(name);
		free(caminho);
		free(biblio);
		return TABLE_NOT_FOUND;
		 // fecha arquivo do dicionario
	}
	
	FILE *meta=fopen("files/fs_coluna.dat","r");// abre meta-dados
	if(meta == NULL) return FILE_META_NOT_FOUND;

	FILE *arquivo = fopen(caminho,"r"); // abre arquivo de dados
	if(arquivo == NULL) return FILE_DATA_NOT_FOUND;

	
	int copiar = 0;
	
	fseek(meta,0,SEEK_END);
	total = ftell(meta);
	long pos = -1;
	rewind(meta); 
	
	 
	while( copiar != biblio->id && i < total){ //Procura a posicao inicial do metadados
		fread( &copiar, sizeof(int), 1, meta);
		if(copiar == biblio->id){
			pos = ftell(meta) - sizeof(int);
			break;
		}
			
		else
		fseek( meta, CONST_META - sizeof(int) , SEEK_CUR);
		i +=ftell(meta);
		
	}
	
	if( pos == -1)
		return TABLE_NOT_FOUND; //Table Not Found
	
	int fieldCount = counter( pos, meta, total, biblio); //Números de atributos da tabela
	
	fseek(meta, pos, SEEK_SET); //Seta o ponteiro para o início dos atributos da tabela 

	
	field *fieldList = NULL;
	if(( fieldList = malloc(sizeof(field) * fieldCount))==NULL){
			return OUT_MEMORIA;
	}; // aloca lista com o numero de campos que tem a tabela

	
	if(!bufferPool){ //recebe ponteiro null para saber quando deve ser inicializado
		bufferPool = (buffer **)malloc(sizeof(buffer));
		if(!bufferPool)
			return OUT_MEMORIA;
		initBuffer(*bufferPool, BUFFER_SIZE , fieldList, fieldCount);
	}
	
		
	//int achou = 0;
	i = 0;
	int j,breakPoint = 0;
	int tupleLenght = 0;
	
	
    
	for(i = 0; i < fieldCount; i++){
		fseek( meta, sizeof(int), SEEK_CUR);
		fread(&fieldList[i].fName,sizeof(char),TNAME_LENGHT, meta);
		fread(&fieldList[i].fType,sizeof(char),1,meta);
		fread(&fieldList[i].fLenght,sizeof(int),1,meta);
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
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
<<<<<<< HEAD
	tupleLenght += fieldCount;//até aqui vamos deixar no arquivo colunas.dat
	//Cria os campos temporários para a montagem da tupla
	fclose(arqm);
=======
	
	
	
	contador = getTupleNumber(arquivo,contador,tupleLenght);
	tupleLenght += (fieldCount);//até aqui vamos deixar no arquivo colunas.dat
	//Cria os campos temporários para a montagem da tupla
	fclose(meta);
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
	int *tInt = NULL;
	if((tInt=malloc(sizeof(int)))==NULL){
		return OUT_MEMORIA;
	};
	double *tDouble = NULL;
	if((tDouble=malloc(sizeof(double)))==NULL){
		return OUT_MEMORIA;
	};
	char *tChar = NULL;
	char *tTuple = NULL;
	if((tTuple=malloc(sizeof(char)*tupleLenght))==NULL){
		return OUT_MEMORIA;
	};
<<<<<<< HEAD
	 
=======
	
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
	char caracter;
	fseek(arquivo, contador, SEEK_CUR);return 0;
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
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
				if (i != (fieldCount-1))
					tTuple[thePointer++] = '*';
					
				 //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] = '#';
				//se acabo a tupla.
				
				//tTuple[thePointer++] = '\0';
				
				
			}
			else if(fieldList[i].fType == 'D')
			{
				//Double
				fread(tDouble,sizeof(double),1,arquivo); 
				vdouble.numd = *tDouble; 
				cpyvar(tTuple,vdouble.cnumd,thePointer,sizeof(double));
				thePointer += sizeof(double);
				if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] =  '#'; //se acabo a tupla.
			
				//tTuple[thePointer++] = '\0';
			}
			else if(fieldList[i].fType == 'S')
			{
				
				//String
				if((tChar = (char *)malloc(sizeof(char) * fieldList[i].fLenght))==NULL){
					return OUT_MEMORIA;
<<<<<<< HEAD
				};
				fread(tChar,sizeof(char),fieldList[i].fLenght,arq);
				sprintf(tTuple, "%s%s",tTuple,tChar);
				if (i != (fieldCount-1)) strcat(tTuple, "*");
				else if (i == (fieldCount-1)) strcat(tTuple, "#");
=======
				}; 
				fread(tChar,sizeof(char),fieldList[i].fLenght,arquivo);
				
				cpyvar( tTuple, tChar, thePointer, fieldList[i].fLenght);
				thePointer =+ fieldList[i].fLenght;
				if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] = '#'; //se acabo a tupla.
				free(tChar);
				
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
			}
			else if(fieldList[i].fType == 'C')
			{
				//Caracter
<<<<<<< HEAD
				if((tChar = (char *)malloc(sizeof(char)))==NULL){
					return OUT_MEMORIA;
				};
				fread(tChar,sizeof(char),1,arq);
				sprintf(tTuple, "%s%s",tTuple,tChar);
=======
				char c;
				fread(&c,sizeof(char),1,arquivo);
				//tChar[1] = '\0'; //Trata caracter como string
				
					tTuple[thePointer++] = c;
					
				if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] ='#'; //se acabo a tupla.
				
			
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
			}
		}
		/*puts("vvvvvvvvvv");
		printf("%c", tTuple[0]);
		printf("%c", tTuple[1]);
		printf("%c", tTuple[2]);
		printf("%c", tTuple[3]);
		printf("%c", tTuple[4]);
		printf("%c", tTuple[5]);
		printf("%c", tTuple[15]);
		double *l = (double *)&tTuple[16];
		printf("%f", *l);
		printf("%c", tTuple[24]);
		int *d = (int *)&tTuple[25];
		printf("V: %d", *d);
		printf("%c", tTuple[29]);
		printf("%c", tTuple[30]);
		printf("%c", tTuple[31]);
		fclose(arquivo);fclose(tabela);return 0;*/
	/*	//Se o arquivo de dados chegar ao fim, as tuplas param de ser entregues ao bufferPool
		if(feof(arq)) break;
<<<<<<< HEAD
		i = 0;
		if(bufferInsert(bufferPool,tTuple,ftell(arq)-tupleLenght,tupleLenght)==OUT_MEMORIA)
			return OUT_MEMORIA;
	};
	//Fecha o arquivo de dados
	fclose(arq);
	free(name);
	free(caminho);
	free(tInt);
	free(tChar);
=======
		i = 0;*/
		if(bufferInsert(*bufferPool,tTuple,ftell(arquivo)-tupleLenght,tupleLenght)==OUT_MEMORIA)
			return OUT_MEMORIA;
	
	//Fecha o arquivo de dados
	fclose(arquivo);
	free(name);
	free(caminho);
	free(tInt);
	//free(tChar);
>>>>>>> 5bcf99ca9ea795ed58db5981362d1ae83a0e9d8c
	free(tDouble);
	free(tTuple);
	return BUFFER_PREENCHIDO;

}
 
