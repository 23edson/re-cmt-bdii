
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
	strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
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
int counter( int init, FILE *metadados, int total, criar myTable ){
	//Essas Função conta o número de atributos de uma tabela
	int copiar = myTable->id;
	int AttCount = 0;
	i = init;
	while( copiar == myTable->id && i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);//printf("cop: %d  e %ld", copiar, ftell(metadados));
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
	}
	
	return AttCount;
}
int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador)
{
	
	//Abertura dos arquivos de dados e de metadados
	char *name = NULL; //Name representa o nome da tabela
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
	
	int i = 0, achou=0;
	
	FILE *tabela = fopen("files/fs_tabela.dat","r");
	if(tabela== NULL) return FILE_NOT_FOUND;
	
	do {
		fread(&biblio->id,sizeof(int),1,fp);
		if(feof(tabela)==0){
			break;
		}
		fread(biblio->lnome,sizeof(char),CONST,fp);
		fread(biblio->fnome,sizeof(char),CONST,fp);
		fread(biblio->dir,sizeof(char),CONST,fp);
		if((strcmp(biblio->lnome,nomeTabela))==0){	
			achou=1;
			i=0;
			strcat(caminho,biblio->dir);
			strcat(caminho,biblio->fnome);
			break; // sai do loop pois encontrou a tabela
		}
		else{ 
				memset(name,0,strlen(name));
				i=0;
		}
	}while(!feof(fp));
	if (achou == 0){ 
		fclose(tabela);
		return TABLE_NOT_FOUND;
		 // fecha arquivo do dicionario
	}

	FILE *meta=fopen("files/fs_colunas.dat","r");// abre meta-dados
	if(meta == NULL) return FILE_META_NOT_FOUND;

	FILE *arquivo = fopen(caminho,"r"); // abre arquivo de dados
	if(arquivo == NULL) return FILE_DATA_NOT_FOUND;

	
	int copiar = 0;
	
	fseek(meta,0,SEEK_END);
	long total = ftell(meta);
	long pos -1;
	rewind(meta);
	 
	while( copiar != biblio->id && i < total){ //Procura a posicao inicial do metadados
		fread( &copiar, sizeof(int), 1, metadados);
		if(copiar == myTable->id){
			pos = ftell(metadados) - sizeof(int);
			break;
		}
			
		else
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		i +=ftell(metadados);
		
	}
	if( pos == -1)
		return TABLE_NOT_FOUND; //Table Not Found
	
	int fieldCount = counter( pos, meta, total, biblio); //Números de atributos da tabela

	//Lê a quantidade de campos no arquivo de metadados
	
	if((fieldList = malloc(sizeof(field) * fieldCount))==NULL){
			return OUT_MEMORIA;
	}; // aloca lista com o numero de campos que tem a tabela

	
	if(!bufferPool){ //recebe ponteiro null para saber quando deve ser inicializado
		bufferPool = (buffer **)malloc(sizeof(buffer));
		initBuffer(*bufferPool, BUFFER_SIZE , fieldList, fieldCount);
	}
	else{
		
	
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
		//Vai montando o tamanho da tupla com base nos tamanhos dos campos encontrados
		tupleLenght += fieldList[i].fLenght;
	}
	tupleLenght += fieldCount;//até aqui vamos deixar no arquivo colunas.dat
	//Cria os campos temporários para a montagem da tupla
	fclose(arqm);
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

	//Começa a leitura dos dados.
	for(strcpy(tTuple,"");;strcpy(tTuple,""))
	{
		//Cada tupla lida é inserida no buffer
		for(i = 0; i < fieldCount; i++)
		{
			//Cada campo de tupla é checado seu tipo e adicionado corretamente na tupla temporária *tTuple
			if(fieldList[i].fType == 'I')
			{
				//Inteiro
				fread(tInt,sizeof(int),1,arq);
				sprintf(tTuple, "%s%d",tTuple,*tInt);
				if (i != (fieldCount-1)) strcat(tTuple, "*");
				else if (i == (fieldCount-1)) strcat(tTuple, "#");
			}
			else if(fieldList[i].fType == 'D')
			{
				//Double
				fread(tDouble,sizeof(double),1,arq);
				sprintf(tTuple, "%s%lf",tTuple,*tDouble);
			}
			else if(fieldList[i].fType == 'S')
			{
				//String
				if((tChar = (char *)malloc(sizeof(char) * fieldList[i].fLenght))==NULL){
					return OUT_MEMORIA;
				};
				fread(tChar,sizeof(char),fieldList[i].fLenght,arq);
				sprintf(tTuple, "%s%s",tTuple,tChar);
				if (i != (fieldCount-1)) strcat(tTuple, "*");
				else if (i == (fieldCount-1)) strcat(tTuple, "#");
			}
			else if(fieldList[i].fType == 'C')
			{
				//Caracter
				if((tChar = (char *)malloc(sizeof(char)))==NULL){
					return OUT_MEMORIA;
				};
				fread(tChar,sizeof(char),1,arq);
				sprintf(tTuple, "%s%s",tTuple,tChar);
			}
		}
		//Se o arquivo de dados chegar ao fim, as tuplas param de ser entregues ao bufferPool
		if(feof(arq)) break;
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
	free(tDouble);
	free(tTuple);
	return BUFFER_PREENCHIDO;
}
}
 
