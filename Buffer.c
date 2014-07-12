
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
	

void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount){
	int i;
	//Inicializa o buffer com todos os diskSeek em -1 para ser a flag que verifica se está em uso ou não.
	for(i = 0; i < lenght; i++){
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
Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber){
	int i;
	//struct Element *arr =  * 3);
	Element_t *myTuple = NULL;
	int sum = 0;
	if(!bufferPool)
	//puts("xxx");
	if(tupleNumber < 1 || tupleNumber > bufferPool->countItems)
		return NULL;
	
	tupleNumber--;
	//printf("Bu; %d,%d,%d,%d", bufferPool->nextPageAvaliable,bufferPool->countItems,bufferPool->fieldCount, bufferPool->bp[0].diskSeek);;
	myTuple = (Element_t *)malloc(sizeof(Element_t) * bufferPool->fieldCount);
	if(!myTuple)
		return NULL;
	
	for(i = 0; i < bufferPool->fieldCount; i ++){
		if(bufferPool->fieldList[i].fType == 'S'){
			myTuple[i].type = String;
			myTuple[i].Str = (char *) malloc(sizeof(char)*bufferPool->fieldList[i].fLenght);
			cpyvar(myTuple[i].Str, bufferPool->bp[tupleNumber].data, sum, bufferPool->fieldList[i].fLenght);
			sum += bufferPool->fieldList[i].fLenght;
		}
		else if(bufferPool->fieldList[i].fType == 'I'){
			myTuple[i].type = Nint;
			myTuple[i].Dint = (int *)&bufferPool->bp[tupleNumber].data[sum];
			sum += sizeof(int);
		}
		else if(bufferPool->fieldList[i].fType == 'D'){
			myTuple[i].type = Ndouble;
			myTuple[i].Ddouble = (double *)&bufferPool->bp[tupleNumber].data[sum];
			sum += sizeof(double);
		}
		else{
			myTuple[i].type = Caracter;
			myTuple[i].Str = malloc(sizeof(char));
			*myTuple[i].Str =bufferPool->bp[tupleNumber].data[sum];
			sum += sizeof(char);
		}	
		//myTuple[i].tipo = bufferPool->fieldList[i].fType;
		 //printf("%s",bufferPool->fieldList[i].fName);
		
	}
	/*printf("\n");
	puts(myTuple[0].Str);
	printf("double: %f", *myTuple[1].Ddouble);
	printf("int : %d", *myTuple[2].Dint);
	printf("caracter: %c", *myTuple[3].Str);
	printf("%c", bufferPool->bp[0].data[0]);
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
	//printf("%p", myTuple);
	return myTuple;
}

void findNextAvaliable(buffer *bPool){
	int i;
	//Simples verificação se a página está disponível se do diskSeek for igual à -1
	for(i = 0; i < BUFFER_SIZE; i++){
		if(bPool->bp[i].diskSeek == -1){
			bPool->nextPageAvaliable = i;
			break;
		}

	}
}

void applyReplacementPolicies(buffer *bPool){
	//Aqui é escolhido a página do buffer que tem o menor pinCount para ser substituida
	int i, lower = bPool->bp[0].pinCount;
	bufferPage bPage = bPool->bp[0];
	for(i = 0; i < BUFFER_SIZE; i++){
		if(bPool->bp[i].pinCount < lower){
			bPage = bPool->bp[i];
			lower = bPool->bp[i].pinCount;
		}
	}
	//A página encontrada fica armazenada em bPage
	bPage.rewriteBit = 1;
	if(bPage.rewriteBit == 0){
		bPool->bp[i].diskSeek = -1;
		bPool->nextPageAvaliable = i;
	}
	else{
		//Aqui deve ser colocado o código para regravação do arquivo.

	}
}
int bufferInsert(buffer *bPool,char *tupla, int diskSeek, int tupleLenght){
	
	if(!bPool)
		//puts("nao");
	//Primeiro verifica se o buffer está cheio, caso esteja ele aplica as políticas de troca
	if(bPool->countItems >= BUFFER_SIZE){
		applyReplacementPolicies(bPool);//puts("aquixxxxxxxxxxxxxxxxx");
	}
	//É inserido na próxima página disponível
	bPool->bp[bPool->nextPageAvaliable].diskSeek = diskSeek;
	if((bPool->bp[bPool->nextPageAvaliable].data = (char *)malloc(tupleLenght*sizeof(char)))==NULL){
		return OUT_MEMORIA;
	}
	cpyvar(bPool->bp[bPool->nextPageAvaliable].data, tupla, 0, tupleLenght);
	//strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
	bPool->bp[bPool->nextPageAvaliable].pinCount = 0;
	bPool->bp[bPool->nextPageAvaliable].rewriteBit = 0;
	bPool->countItems ++;
	//É procurado a próxima página disponível
	findNextAvaliable(bPool);
	return 0;
}
int showBuffer(buffer *bufferPool,int page){
	if (!bufferPool->countItems) return EMPTY_BUFFER;
	//Primeiro é mostrado os campos
	int i, j=0;
	printf("\n");
	for(i = 0; i < bufferPool->fieldCount; i ++){
		printf("%s",bufferPool->fieldList[i].fName);
		if (i==0) printf("\t");  
		printf("\t");
	}
	printf("\n");
	//Depois é mostrado o conteúdo do buffer
	for(i = 0; i < BUFFER_SIZE; i++){
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

int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador){
	
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
	
	FILE *meta=fopen("files/fs_coluna.dat","r");// abre meta-dados
	if(meta == NULL) return FILE_META_NOT_FOUND;

	FILE *arquivo = fopen(caminho,"r"); // abre arquivo de dados
	if(arquivo == NULL) return FILE_DATA_NOT_FOUND;

	
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
	
	if( pos == -1)
		return TABLE_NOT_FOUND; //Table Not Found
	//printf("posss : %ld", pos); 
	int fieldCount = counter( pos, meta, total, biblio); //Números de atributos da tabela
	
	fseek(meta, pos, SEEK_SET); //Seta o ponteiro para o início dos atributos da tabela 

	
	field *fieldList = NULL;
	if(( fieldList = malloc(sizeof(field) * fieldCount))==NULL){
			return OUT_MEMORIA;
	}; // aloca lista com o numero de campos que tem a tabela

	
	if(!*bufferPool){ //recebe ponteiro null para saber quando deve ser inicializado
		//puts("entrei");
		
		*bufferPool = (buffer *)malloc(sizeof(buffer));
		
		if(!*bufferPool)
			return OUT_MEMORIA;
		
		initBuffer(*bufferPool, BUFFER_SIZE , fieldList, fieldCount);
	}
	
		
	//int achou = 0;
	i = 0;
	
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
	
	
	
	contador = getTupleNumber(arquivo,contador,tupleLenght);
	if(contador == TUPLE_NOT_FOUND)
		return contador;
	//até aqui vamos deixar no arquivo colunas.dat
	//Cria os campos temporários para a montagem da tupla
	fclose(meta);
	int *tInt = NULL;
	if((tInt=malloc(sizeof(int)))==NULL){
		return OUT_MEMORIA;
	}
	double *tDouble = NULL;
	if((tDouble=malloc(sizeof(double)))==NULL){
		return OUT_MEMORIA;
	}
	char *tChar = NULL;
	char *tTuple = NULL;
	if((tTuple=malloc(sizeof(char)*tupleLenght))==NULL){
		return OUT_MEMORIA;
	}
	
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
	//printf("inicio:%d", contador);
	
	fseek(arquivo, contador, SEEK_SET);
	//Começa a leitura dos dados.
	
		//Cada tupla lida é inserida no buffer
		for(i = 0; i < fieldCount; i++){
			//Cada campo de tupla é checado seu tipo e adicionado corretamente na tupla temporária *tTuple
			if(fieldList[i].fType == 'I'){
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
			else if(fieldList[i].fType == 'D'){
				//Double
				fread(tDouble,sizeof(double),1,arquivo); 
				vdouble.numd = *tDouble; 
				cpyvar(tTuple,vdouble.cnumd,thePointer,sizeof(double));
				thePointer += sizeof(double);
				/*if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] =  '#'; //se acabo a tupla.*/
			
				//tTuple[thePointer++] = '\0';
			}
			else if(fieldList[i].fType == 'S'){
				
				//String
				if((tChar = (char *)malloc(sizeof(char) * fieldList[i].fLenght))==NULL){
					return OUT_MEMORIA;
				}; 
				fread(tChar,sizeof(char),fieldList[i].fLenght,arquivo);
				
				cpyvar( tTuple, tChar, thePointer, fieldList[i].fLenght);
				thePointer =+ fieldList[i].fLenght;
				/*if (i != (fieldCount-1)) tTuple[thePointer++] = '*'; //se nao acaba a tupla, mas apenas o atributo
				else if (i == (fieldCount-1)) tTuple[thePointer++] = '#'; //se acabo a tupla.*/
				free(tChar);
				
			}
			else if(fieldList[i].fType == 'C'){
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
		//printf("%c", tTuple[29]);
		printf("%c\n\n\n", tTuple[27]);
		//printf("%c", tTuple[31]);
		
		fclose(arquivo);fclose(tabela);return 0;
		//Se o arquivo de dados chegar ao fim, as tuplas param de ser entregues ao bufferPool
		if(feof(arq)) break;
		i = 0;*/
		//puts("there");
		if(bufferInsert(*bufferPool,tTuple,0,tupleLenght)==OUT_MEMORIA)
			return OUT_MEMORIA;
	
	//Fecha o arquivo de dados
	fclose(arquivo);
	free(name);
	free(caminho);
	free(tInt);
	//free(tChar);
	free(tDouble);
	free(tTuple);
	return OKAY;

}
 
int searchTable(FILE *arquivo, char *Tabela, criar **myTable){
	
	if(!arquivo)
		return FILE_NOT_FOUND;
	int contador = 0;
	criar *table = (criar *)malloc(sizeof(criar));
	if(!table)
		return 0;
	
	fseek( arquivo, 0, SEEK_END);
	int total = ftell(arquivo);
	rewind(arquivo);
	
	for(;contador < total;){
		fread( &(*myTable)->id, sizeof(int), 1, arquivo);	
		fread( (*myTable)->lnome, sizeof(char), CONST, arquivo);
		fread( (*myTable)->fnome, sizeof(char), CONST, arquivo);
		fread( (*myTable)->dir, sizeof(char), CONST, arquivo);
		
		if( strcmp( (*myTable)->lnome, Tabela) == 0) {//caso encontrou a tabela
			free(table);
			fclose(arquivo);
			return OKAY;
		} 
		contador += CONST_MAX ;
	}
	free(table);
	fclose(arquivo);
	return TABLE_NOT_FOUND; //erro table not found
}
		
		
int testeTam( int numero, int count){
	//Essa função retorna a quantidade de digitos de um numero
	int Pgroup = 10;
	int Agroup = 10;
	//int LastGroup;
	int limit = count;
	count = 1;
	
	while( count <= limit){
		
		if( numero < Pgroup){
			if(count == limit)
				return OKAY;
			
		}
		Pgroup *= Agroup;
		//LastGroup = Pgroup;
		count++;
	}
	return VIOLATE_NUMBER_LENGTH;
}
int verifyCaracter( char nome){
	//Vefirica se o caracter é uma letra
	if((nome >= 'a' && nome <= 'z') || (nome >='A' && nome <= 'Z')) // de A até z na tabela ascii
		return 1;
	
	return 0;

}
int checkMeta( field estrutura[], int numberAtt){
	//Checa algumas situações
	int i = 0;
	int j = 0;
	
	
	while( j < numberAtt){ 
		while( estrutura->fName[i] != '\0'){//printf("%c ", estrutura.fName[i]);
			if( !verifyCaracter(estrutura->fName[i])) // de A até z na tabela ascii
				return VALOR_INVALIDO;
			
				i++;
		}
		i=0;
		if(!(estrutura->fType == 'S' || estrutura->fType == 'I') || (estrutura->fType == 'D' || estrutura->fType == 'C'))	
			return VALOR_INVALIDO;
		j++;
	}
	return OKAY;
}

int createAttibutes( int id, field *atributos, int numberAtt){
	//Essa função cria os elementos da tabela criada, conforme passada por parâmetro
	
	/*struct meta{
		int id;
		char atnome[40];
		char tipo;
		int tam;
	};
	typedef struct meta META;*/
	
	FILE *metadados = fopen("files/fs_coluna.dat", "a+");  // a+ ( abre o arquivo para R-W, caso não existir, ele será criado.)
	if(!metadados)
		return FILE_NOT_FOUND;
	
	//int count = sizeof(atributos) / sizeof(atributos[0]); //Quantidade de índices que a variável atributos possuí
	
	//printf("id %d e T:%d", id,numberAtt);
	int i = 0;
	int getErro;
	int valor;
	
	fseek(metadados,0,SEEK_END); 
	long int pos = ftell(metadados);//tamanho do arquivo
	rewind(metadados);//printf("Th:%ld", pos);
	
	while( i < pos){ //Procura se já existe  o id criado no arquivo
		
		fread(&valor,sizeof(int),1,metadados);
		if( valor == id){
			return VALUE_ALREADY_EXISTS;
		}
		i += CONST_META;
		fseek( metadados, CONST_META - sizeof(int), SEEK_CUR);
	}
	i = 0;
	//int tNumber = 1;
		
	while( i < numberAtt){ //caso não existe, será gravado no arquivo os valores passados pelo parâmetro
		
				
			
		if((getErro = checkMeta(atributos, numberAtt) == OKAY)){ //Checa alguns formatos válidos nos atributos
			fwrite( &id, sizeof(int), 1 , metadados);
			
			fwrite( atributos[i].fName, sizeof(char), TNAME_LENGHT, metadados);
			fwrite( &atributos[i].fType, sizeof(char), 1, metadados);
			fwrite( &atributos[i].fLenght, sizeof(int),1, metadados);
			i++;
		}
			
		else{ fclose(metadados);  return getErro;}
	}
	/*struct meta{
		int id;
		char atnome[40];
		char tipo;
		int tam;
	};
	typedef struct meta META;
	
	META di[4];
	rewind(metadados);
	
	fread(&di[0].id, sizeof(int),1,metadados);
	fread(di[0].atnome, sizeof(char),TNAME_LENGHT,metadados);
	fread(&di[0].tipo, sizeof(char),1,metadados);
	fread(&di[0].tam, sizeof(int),1,metadados);
	
	fread(&di[1].id, sizeof(int),1,metadados);
	fread(di[1].atnome, sizeof(char),TNAME_LENGHT,metadados);
	fread(&di[1].tipo, sizeof(char),1,metadados);
	fread(&di[1].tam, sizeof(int),1,metadados);
	
	fread(&di[2].id, sizeof(int),1,metadados);
	fread(di[2].atnome, sizeof(char),TNAME_LENGHT,metadados);
	fread(&di[2].tipo, sizeof(char),1,metadados);
	fread(&di[2].tam, sizeof(int),1,metadados);
	
	fread(&di[3].id, sizeof(int),1,metadados);
	fread(di[3].atnome, sizeof(char),TNAME_LENGHT,metadados);
	fread(&di[3].tipo, sizeof(char),1,metadados);
	fread(&di[3].tam, sizeof(int),1,metadados);
			
	printf("\n id : %d\n", di[0].id);
	puts(di[0].atnome);
	printf("\n Tipo : %c\n", di[0].tipo);
	printf("\n tam : %d\n", di[0].tam);
	
	printf("\n id2 : %d\n", di[1].id);
	puts(di[1].atnome);
	printf("\n Tipo2 : %c\n", di[1].tipo);
	printf("\n tam2 555: %d\n", di[1].tam);
	
	printf("\n id2 : %d\n", di[2].id);
	puts(di[2].atnome);
	printf("\n Tipo2 : %c\n", di[2].tipo);
	printf("\n tam2 : %d\n", di[2].tam);
	
	printf("\n id : %d\n", di[3].id);
	puts(di[3].atnome);
	printf("\n Tipo : %c\n", di[3].tipo);
	printf("\n tam : %d\n", di[3].tam);*/
	
	fclose(metadados);
	return OKAY;
}
 char *GeneratePhysName( char *last){
	//A finalidade dessa função é gerar um nome para o arquivo de dados da tabela x. Ex: se a o nome do ultimo arquivo foi : file_1.dat
	//Então o próximo será file_2.dat
	char caracter = 'a';
	bool isNumber = false;
	char *number;
	int shift = 0;
	int i = 0;
	number = (char *) (malloc(sizeof(char)*2));
	
	if(!number){
		return NULL;
	}
	int max = strlen(last);
	
	while( i < max){ //Este while procura o número do arquivo dentro da string.
		caracter = last[i]; //Assuma que o número sempre estará após o underscore e antes do .dat
		if(caracter == '_'){
			isNumber = true;
		}
		else if(isNumber){
			number[shift] = caracter;
			shift++;
			if(last[i+1] == '.'){
				number[shift] = '\0';
				i = max;
			}
		}
		i++;
	}
	
	
	//number[shift] = '\0';
	long int num = strtol(number, NULL, BASE); //Conversao da string apontada por (number) em inteiro
	
	num++;
	
	char *fnome;
	
	fnome = ( char *) malloc(sizeof(char)* CONST);
	if(!fnome)
		return NULL;
	
    
    char const digitos[] = "0123456789";
	char* pointer = fnome;
    //Este trecho pega um número inteiro e faz a representação dele em forma de string
    //Apenas usada para geração do nome do arquivo de dados
		shift = num;
    do{ //Acha o final da representação do número em string
        ++pointer;
        shift = shift/10;
    }while(shift);
    *pointer = '\0';
    do{ //Cálculo para pegar a representação da variável num
        *--pointer = digitos[num%10];
        num = num/10;
     
    }while(num);
    
    char *string;
    string = (char *)malloc(sizeof(char)*20);
    strcpy(string, "file_");
	strcat( string, fnome);
	strcat( string, ".dat");
	
	free(fnome);
	free(number);
	
	return string;
	
}

int createTable( char *TableName, field *Attributes, int numberAtt){
	
	if(Attributes == NULL || numberAtt <= 0)
		return ABORT;
	
	FILE *arquivo = fopen("files/fs_tabela.dat", "a+"); //Localização fixa
	
	if(!arquivo){
		return FILE_NOT_FOUND;}
	
	fseek(arquivo,0,SEEK_END); 
	long total = ftell(arquivo); //Tamanho do arquivo
	//printf("Total: %ld\n", total);	
	rewind(arquivo); //coloca o ponteiro na posição zero
	int i = 0;
	
	criar *ConstTab = malloc(sizeof(criar));
	if(!ConstTab){
		return 0;
	}
	ConstTab->id = 0;
	strcpy(ConstTab->fnome,"file_0\0");
	strcpy(ConstTab->dir, "files/data/\0"); //Valores default
	
	for(;i < total;){ //Este laço percorre o arquivo e procura se já existe a tabela
		fread( &ConstTab->id, sizeof(int), 1, arquivo);	
		fread( ConstTab->lnome, sizeof(char), CONST, arquivo);
		fread( ConstTab->fnome, sizeof(char), CONST, arquivo); //puts(ConstTab->fnome);
		fread( ConstTab->dir, sizeof(char), CONST, arquivo);
		
		if( strcmp( ConstTab->lnome, TableName) == 0) {free(ConstTab);return TABLE_ALREADY_EXISTS;} //caso a tabela já exista
		//fseek( arquivo, CONST*2, SEEK_CUR);
		i += CONST_MAX ; //printf("val %d ", i);
	}
	//printf("Valor:%d", count);
	ConstTab->id ++;
	strcpy(ConstTab->lnome, TableName);
	//puts(ConstTab->fnome);

	char *getFromReturn = GeneratePhysName(ConstTab->fnome);
	if(!getFromReturn)
		return 0;
		
	strcpy( ConstTab->fnome, getFromReturn);//puts(ConstTab->fnome);
	
	fwrite(&ConstTab->id, sizeof(int), 1, arquivo);
	fwrite(ConstTab->lnome, sizeof(char), CONST, arquivo);
	fwrite(ConstTab->fnome, sizeof(char), CONST, arquivo);
	fwrite(ConstTab->dir, sizeof(char), CONST, arquivo);
	
	/*rewind(arquivo);
	fseek(arquivo, 0, SEEK_CUR);
	
	criar ConstTab1;
	
	fread(&ConstTab1.id, sizeof(int), 1 , arquivo);
	fread(ConstTab1.lnome, sizeof(char),CONST, arquivo);
	fread(ConstTab1.fnome, sizeof(char), CONST, arquivo);
	fread(ConstTab1.dir,sizeof(char), CONST, arquivo);
	
	printf("Id:%d\n", ConstTab1.id);
	puts(ConstTab1.lnome);
	puts(ConstTab1.fnome);
	puts(ConstTab1.dir);*/
	
	int getErro = createAttibutes( ConstTab->id, Attributes, numberAtt);
	//criar o arquivo
	
	fclose(arquivo);
	free(ConstTab);
	free(getFromReturn);
	
	return getErro;
}
int insertInto( char *tableName, Element_t *Attributes){
	
	if(!Attributes || !tableName)
		return ABORT;
	
		
	FILE *table = fopen("files/fs_tabela.dat", "r");
	if(!table)
		return FILE_NOT_FOUND;
	
	FILE *metadados = NULL;
	FILE *newFile = NULL;;
	
	int copiar = 0;
	long int pos = -1;
	int i = 0;
	
	char *diretorio = NULL;
	criar *myTable = NULL;
	myTable = (criar *)malloc(sizeof(criar));
	
	
	int getErro = searchTable(table, tableName, &myTable);
	
	
	if(getErro != OKAY)
		return getErro;
	
	
	
	metadados = fopen("files/fs_coluna.dat", "r");
	if(!metadados)
		return FILE_NOT_FOUND;
		
	fseek(metadados,0,SEEK_END);
	long total = ftell(metadados); //Tamanho do arquivo
	rewind(metadados);
		
	//copiar = myTable->id;
	field *mDados;
	int AttCount = 0;
	while(i < total){ //Procura a posicao inicial do metadados
	
		fread( &copiar, sizeof(int), 1, metadados);
		
		if(copiar == myTable->id){
			pos = ftell(metadados) - sizeof(int);
			break;
		}
			
		else{
			fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
			i += CONST_META;
		}
	}
	if( pos == -1)
		return TABLE_NOT_FOUND; //Table Not Found
		
	fseek(metadados, pos, SEEK_SET);
	i = pos;
	while(i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);//printf("cop: %d  e %ld", copiar, ftell(metadados));
		if(myTable->id != copiar)
			break;
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
	}
	mDados = (field *)malloc(sizeof(field) * AttCount);
	
	fseek(metadados, pos, SEEK_SET);
	
	copiar = 0;
	int endLoop = 0;
	int globalID;
	while( (endLoop < total) && (copiar < AttCount)){
		fread( &globalID, sizeof(int), 1, metadados);
		fread( mDados[copiar].fName, sizeof(char), TNAME_LENGHT, metadados);
		fread( &mDados[copiar].fType, sizeof(char),1, metadados);
		fread( &mDados[copiar].fLenght, sizeof(int), 1, metadados);
		
		copiar++;
		
		endLoop += CONST_META;
	}
	
	/*if(Quantidade != AttCount) //caso o número de atributos no parâmetro for menor ou maior que a quantidade de att.
		return ERRO_ATT_NUMBER;
	*/	
	copiar = 0;
	
	while( copiar < AttCount){
	//	int numero = strtol( Attributes[copiar].tValue, NULL, BASE);
				
		if(mDados[copiar].fType == 'I' && Attributes[copiar].type == Nint){
			
			if(testeTam(*Attributes[copiar].Dint, mDados[copiar].fLenght) != OKAY){
				free(mDados);
				free(diretorio);
				free(myTable);
				fclose(metadados);
				
				return VIOLATE_NUMBER_LENGTH;
			}
		}
		copiar++;
	}
	diretorio = (char *)malloc(sizeof(char)* CONST);
	
	strcpy(diretorio, myTable->dir);
	strcat(diretorio, myTable->fnome);
	newFile = fopen(diretorio, "a+");
	
	if(!newFile){
		if(!(newFile=fopen(diretorio,"w+"))){
			return FILE_NOT_FOUND;
		}
	}
	copiar = 0; 
	
	
	while( copiar < AttCount){
		
		if(mDados[copiar].fType == 'S' && Attributes[copiar].type == String){
			//printf("val :%d, %d ", mDados[copiar].fLenght,copiar);
			//char sg[15];
			//strcpy(sg, );
			//puts(sg);
			fwrite( Attributes[copiar].Str,sizeof(char), mDados[copiar].fLenght, newFile);
		}
		else if( mDados[copiar].fType == 'C' && Attributes[copiar].type == Caracter){ 
			fwrite(Attributes[copiar].Str, sizeof(char),1, newFile);
		}	
		else if(mDados[copiar].fType == 'D' && Attributes[copiar].type == Ndouble){
			//printf("val :%d, %d ", mDados[copiar].fLenght,copiar);
			//double g = *;
			//printf("f: %f", g);
			//double variavel = strtod(Attributes[copiar].tValue,NULL);
			fwrite( Attributes[copiar].Ddouble, sizeof(double), 1, newFile);
		}
		else{
			//int var = strtol( Attributes[copiar].tValue, NULL, BASE);
			fwrite( Attributes[copiar].Dint, sizeof(int),1, newFile);
		}
		copiar++;
	}
	/*fclose(newFile);
	newFile = fopen(diretorio, "r");
	char *at = malloc(sizeof(char)*15);
	int ib;
	double db;
	char cb;
	puts("aq\n");
	fseek(newFile, 28,SEEK_SET);
	fread(at,sizeof(char),15, newFile); puts(at);
	fread(&db, sizeof(double),1 , newFile); printf("%f ", db);
	fread(&ib, sizeof(int),1,newFile); printf("%d ", ib);
	fread(&cb, sizeof(char),1,newFile);printf("%c ", cb);*/
	free(mDados);
	free(diretorio);
	free(myTable);
	fclose(metadados);
	fclose(newFile);
	return OKAY;
}

