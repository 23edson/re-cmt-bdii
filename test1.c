#include"Buffer.h"

/*
 * 
 * @defines constantes
 **/  
#define CONST 30
#define BASE 10
#define CONST_MAX 94 
#define CONST_META 29
#define FILE_NOT_FOUND -5 //provisório
#define VALUE_ALREADY_EXISTS -6
#define VALOR_INVALIDO -7
#define OKAY -1 
#define ABORT -8

typedef struct Ctabela{
		int id;
		char lnome[CONST]; //Estrutura de fs_tabela.dat. De acordo com a parte anterior do trabalho
		char fnome[CONST];
		char dir[CONST];
	}criar;


typedef struct Attribute
{
	char tValue[TNAME_LENGHT];
}Attribute;

typedef struct MultInsert //estruturas para insert ( nao definitiva)
{
	Attribute *nextAtt; 
	Attribute valor;
}MultInsert;

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
		
		if( strcmp( (*myTable)->lnome, Tabela) == 0) {//caso a tabela já exista
			free(table);
			fclose(arquivo);
			return OKAY;
		} 
		contador += CONST_MAX ;
	}
	free(table);
	fclose(arquivo);
	return -84; //erro table not found
}
		
		
int testeTam( int numero, int count){
	//Essa função retorna a quantidade de digitos de um numero
	int Pgroup = 10;
	int Agroup = 10;
	//int LastGroup;
	int limit = count;
	count = 1;
	
	if(limit == 1)
		return 1;
	while( count <= limit){
		
		if( numero < Pgroup)
			return count;
		Pgroup *= Agroup;
		//LastGroup = Pgroup;
		count++;
	}
	return -1;
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
	
			
	while( i < numberAtt){ //caso não existe, será gravado no arquivo os valores passados pelo parâmetro
		if(atributos == NULL)
			return ABORT;
			
			
		if((getErro = checkMeta(atributos, numberAtt) == OKAY)){ //Checa alguns formatos válidos nos atributos
			fwrite( &id, sizeof(int), 1 , metadados);
			fwrite( atributos[i].fName, sizeof(char), TNAME_LENGHT, metadados);
			fwrite( &atributos[i].fType, sizeof(char), 1, metadados);
			fwrite( &atributos[i].fLenght, sizeof(int),1, metadados);
			i++;
		}
			
		else{ fclose(metadados);  return getErro;}
	}
			
	fclose(metadados);
	return OKAY;
}
 char *GeneratePhysName( char *last){
	//A finalidade dessa função é gerar um nome para o arquivo de dados da tabela x. Ex: se a o nome do ultimo arquivo foi : file_1.dat
	//Então o próximo será file_2.dat
	char caracter = 'a';
	int isNumber = 0;
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
			isNumber = 1;
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
		
		if( strcmp( ConstTab->lnome, TableName) == 0) {free(ConstTab);return -84;} //caso a tabela já exista
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
	
	fclose(arquivo);
	free(ConstTab);
	free(getFromReturn);
	
	return getErro;
}
int insertInto( char *tableName, Attribute *Attributes, int Quantidade){
	
	FILE *table = fopen("files/fs_tabela.dat", "r");
	if(!table)
		return FILE_NOT_FOUND;
	
	FILE *metadados = NULL;
	FILE *newFile = NULL;;
	
	int copiar = 0;
	long int pos = -1;
	int i = 0;
	
	criar *myTable = NULL;
	myTable = (criar *)malloc(sizeof(criar));
	
	
	int getErro = searchTable(table, tableName, &myTable);
	
	if(getErro != OKAY)
		return getErro;
	
	char *diretorio = (char *)malloc(sizeof(char)* CONST);
	
	strcat(diretorio, myTable->dir);
	strcat(diretorio, myTable->fnome);
	newFile = fopen(diretorio, "r");
	
	if(!newFile)
		newFile = fopen(diretorio, "a+");
		
	
	metadados = fopen("files/fs_coluna.dat", "r");
	if(!metadados)
		return FILE_NOT_FOUND;
		
	fseek(metadados,0,SEEK_END);
	long total = ftell(metadados); //Tamanho do arquivo
	rewind(metadados);
		
	//copiar = myTable->id;
	field *mDados;
	int AttCount = 0;
	while( copiar != myTable->id && i < total){ //Procura a posicao inicial do metadados
		fread( &copiar, sizeof(int), 1, metadados);
		if(copiar == myTable->id)
			pos = ftell(metadados);
			
		else
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		i +=ftell(metadados);
		
	}
	if( pos == -1)
		return 0; //Table Not Found
		
	while( copiar == myTable->id && i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
	}
	mDados = (field *)malloc(sizeof(field) * AttCount);
	
	fseek(metadados, pos, SEEK_SET);
	
	copiar = 0;
	int endLoop = 0;
	int junk;
	while( (endLoop < total) && (copiar < AttCount)){
		fread( &junk, sizeof(int), 1, metadados);
		fread( mDados[copiar].fName, sizeof(char), TNAME_LENGHT, metadados);
		fread( &mDados[copiar].fType, sizeof(char),1, metadados);
		fread( &mDados[copiar].fLenght, sizeof(int), 1, metadados);
		
		copiar++;
		
		endLoop += CONST_META;
	}
	
	return OKAY;
}


int main(){
	
	
	field *atributos; //o usuario cria um vetor para os atributos que ele ira criar;
	Attribute *inserts;
	//int contador = 0;
	
	
	atributos = (field *) malloc(sizeof(field)* 2); //Neste caso tera dois atributos;
	inserts = (Attribute *) malloc(sizeof(Attribute) * 2);  //Número de atributos que a tabela possui
	
	//int a = sizeof( atributos) / sizeof(atributos[0]);
	//printf("vv%d  " , sizeof(atributos));
	strcpy(atributos[0].fName, "Nome");
	atributos[0].fType = 'S';
	atributos[0].fLenght = 10;
	
	strcpy(atributos[1].fName, "Idade");
	atributos[1].fType = 'I';
	atributos[1].fLenght = 2;  //Para Inteiros fLenght representa a quantidade de digitos
	
	//createTable( "Cliente", atributos,2);
	//checkMeta( atributos[0]);
	//int erro = createTable( "Cliente", atributos, 2);
	//printf("erro : %d ", erro);
	int erro = -1;
	if(erro == OKAY){
	
		strcpy(inserts[0].tValue, "Mathew");
		strcpy(inserts[1].tValue, "35"); //TUPLA 1 PARA A TABELA CLIENTE ( nome, idade)
		
		insertInto("Cliente", inserts, 2);
		
		/*strcpy(inserts[0].tValue, "Andrew");
		strcpy(inserts[1].tValue, "90"); //TUPLA 2 PARA A TABELA CLIENTE ( nome, idade)
		
		insertInto("Cliente", inserts, 2);*/
	}
	/*struct meta{
		int id;
		char atnome[40];
		char tipo;
		int tam;
	};
	typedef struct meta META;
	
	META d[2];
	
	FILE *p= fopen("files/fs_coluna.dat", "r");
	//fseek(p, 116, SEEK_CUR);
	fread(&d[0].id, sizeof(int), 1, p);
	fread(d[0].atnome, sizeof(char), TNAME_LENGHT, p);
	fread(&d[0].tipo, sizeof(char),1,p);
	fread(&d[0].tam,sizeof(int),1,p);
	
	printf("id1: %d\n", d[0].id);
	puts(d[0].atnome);
	printf("tipo1: %c\n", d[0].tipo);
	printf("tam1: %d\n", d[0].tam);
	
	fread(&d[1].id, sizeof(int), 1, p);
	fread(d[1].atnome, sizeof(char), TNAME_LENGHT, p);
	fread(&d[1].tipo, sizeof(char),1,p);
	fread(&d[1].tam,sizeof(int),1,p);
	
	printf("id2: %d\n", d[1].id);
	puts(d[1].atnome);
	printf("tipo2: %c\n", d[1].tipo);
	printf("tam2: %d\n", d[1].tam);
	
	fclose(p);*/
	return 0;
}
