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
#define ERRO_ATT_NUMBER -9
#define VIOLATE_NUMBER_LENGTH -10
#define TABLE_ALREADY_EXISTS -11
#define TABLE_NOT_FOUND -12
struct Ctabela{
		int id;
		char lnome[CONST]; //Estrutura de fs_tabela.dat. De acordo com a parte anterior do trabalho
		char fnome[CONST];
		char dir[CONST];
	};
typedef struct Ctabela criar;


typedef struct Attribute
{
	char tValue[TNAME_LENGHT];
}Attribute;



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
			else break;
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
	
	fclose(arquivo);
	free(ConstTab);
	free(getFromReturn);
	
	return getErro;
}
int insertInto( char *tableName, Attribute *Attributes){
	
	/*if(!Attributes || !tableName)
		return ABORT;*/
	
		
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
	while( copiar != myTable->id && i < total){ //Procura a posicao inicial do metadados
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
		
	fseek(metadados, pos, SEEK_SET);
	i = pos;
	while( copiar == myTable->id && i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);//printf("cop: %d  e %ld", copiar, ftell(metadados));
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
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
	
	/*if(Quantidade != AttCount) //caso o número de atributos no parâmetro for menor ou maior que a quantidade de att.
		return ERRO_ATT_NUMBER;
	*/	
	copiar = 0;
	
	while( copiar < AttCount){
		int numero = strtol( Attributes[copiar].tValue, NULL, BASE);
				
		if(mDados[copiar].fType == 'I'){
			if(testeTam(numero, mDados[copiar].fLenght) != OKAY){
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
	
	if(!newFile)
		return FILE_NOT_FOUND;
	
	copiar = 0; printf("count %d", AttCount);
	while( copiar < AttCount){
		
		if(mDados[copiar].fType == 'S'){
			fwrite( Attributes[copiar].tValue,sizeof(char), mDados[copiar].fLenght, newFile);
		}
		else if( mDados[copiar].fType == 'C'){ 
			fwrite(&Attributes[copiar].tValue[0], sizeof(char),1, newFile);
		}	
		else if(mDados[copiar].fType == 'D'){
			double variavel = strtod(Attributes[copiar].tValue,NULL);
			fwrite( &variavel, sizeof(double), 1, newFile);
		}
		else{
			int var = strtol( Attributes[copiar].tValue, NULL, BASE);
			fwrite( &var, sizeof(int),1, newFile);
		}
		copiar++;
	}
	free(mDados);
	free(diretorio);
	free(myTable);
	fclose(metadados);
	fclose(newFile);
	return OKAY;
}


int main(){
	
	
	field *atributos; //o usuario cria um vetor para os atributos que ele ira criar;
	//Attribute *inserts;
	//int contador = 0;
	
	
	atributos = (field *) malloc(sizeof(field)* 4); //Neste caso tera dois atributos;
	//inserts = (Attribute *) malloc(sizeof(Attribute) * 4);  //Número de atributos que a tabela possui
	
	//int a = sizeof( atributos) / sizeof(atributos[0]);
	//printf("vv%d  " , sizeof(atributos));
	strcpy(atributos[0].fName, "Modelo");
	atributos[0].fType = 'S';
	atributos[0].fLenght = 15;
	
	
	strcpy(atributos[1].fName, "Motor");
	atributos[1].fType = 'D';
	atributos[1].fLenght = 0;  //Para Inteiros fLenght representa a quantidade de digitos
	
	strcpy(atributos[2].fName, "Ano");
	atributos[2].fType = 'I';
	atributos[2].fLenght = 4;
	
	strcpy(atributos[3].fName, "Sigla");
	atributos[3].fType = 'C';
	atributos[3].fLenght = 1;
	int erro = -1;
	erro = createTable("Carro", atributos,4);
	printf("id2: %d\n", erro);
	if(erro == -11){
	
		/*strcpy(inserts[0].tValue, "Corsa");
		strcpy(inserts[1].tValue, "1.6"); //TUPLA 1 PARA A TABELA CLIENTE ( nome, idade)
		strcpy(inserts[2].tValue, "2009");
		strcpy(inserts[3].tValue, "F");
		
		erro = insertInto("Carro", inserts);*/
		
		/*strcpy(inserts[0].tValue, "Andrew");
		strcpy(inserts[1].tValue, "900"); //TUPLA 2 PARA A TABELA CLIENTE ( nome, idade)
		
		erro = insertInto("Cliente", inserts);printf("Ovvvvverro : %d ", erro);*/
	}
	/*struct meta{
		int id;
		char atnome[40];
		char tipo;
		int tam;
	};
	typedef struct meta META;
	
	META di[4];*/
	
	
	Attribute in;

	FILE *m = fopen("files/data/file_1.dat", "r");if(!m) return 2;
	fread( in.tValue, sizeof(char), 15,m); puts(in.tValue);
	double v;
	int g;
	char b;
	fread( &v, sizeof(double), 1,m); printf("m %f\n", v);
	fread( &g, sizeof(int), 1,m); printf("v %d\n", g);
	fread( &b, sizeof(char), 1,m);printf(" f:%c", b);
	
	return 0;
}
