#include"Buffer.h"
#define CONST 30
#define BASE 10

int testeTam( int numero, int count){
	//Essa função retorna a quantidade de digitos de um numero
	int Pgroup = 10;
	int Agroup = 10;
	int LastGroup;
	int limit = count;
	count = 1;
	
	if(limit == 1)
		return 1;
	while( count <= limit){
		
		if( numero < Pgroup)
			return count;
		Pgroup *= Agroup;
		LastGroup = Pgroup;
		count++;
	}
	return -1;
}
 char GeneratePhysName( char *last){
	//A finalidade dessa função é gerar um nome para o arquivo de dados da tabela x. Ex: se a o nome do ultimo arquivo foi : file_1.
	//Então o próximo será file_2.
	long int num = strtol(last, NULL, BASE); //Conversao da string apontada por (last) em inteiro
	
	num++;
	
	char *fnome;
	
	fnome = ( char *) malloc(sizeof(char)* CONST);
	
	
    
    char const digitos[] = "0123456789";
	char* pointer = fnome;
    //Este trecho pega um número inteiro e faz a representação dele em forma de string
    //Apenas usada para geração do nome do arquivo de dados
    int shift = num;
    do{ //Acha o final da representação do número em string
        ++pointer;
        shift = shift/10;
    }while(shift);
    *pointer = '\0';
    do{ //Cálculo para pegar a representação da variável num
        *--pointer = digitos[num%10];
        num = num/10;
     
    }while(num);
    
    char b[20] = "file_\0";
	strcat( b, fnome);
	
	free(fnome);
	
	return *b;
	
}

void createTable( char *TableName, field *Attributes){
	
	struct Ctabela{
		int id;
		char lnome[CONST]; //Estrutura de fs_tabela.dat. De acordo com a parte anterior do trabalho
		char fnome[CONST];
		char dir[CONST];
	};
	typedef struct Ctabela criar;
	
	FILE *arquivo = fopen("files/fs_tabela.dat", "a+"); //Localização fixa
	
	if(!arquivo){
		return;}
	
	fseek(arquivo,0,SEEK_END); 
	long total = ftell(arquivo); //Tamanho do arquivo
	rewind(arquivo); //coloca o ponteiro na posição zero
	
	int i = 0;
	
	criar *ConstTab = malloc(sizeof(criar));
	
	ConstTab->id = 1;
	strcpy(ConstTab->fnome,"000001\0");
	strcpy(ConstTab->dir, "files/data/\0"); //Valores default
	
	for(;i < total; i++){ //Este laço percorre o arquivo e procura se já existe a tabela
		fread( &ConstTab->id, sizeof(int), 1, arquivo);	
		fread( ConstTab->lnome, sizeof(char), CONST, arquivo);
		fread( ConstTab->fnome, sizeof(char), CONST, arquivo);
		fread( ConstTab->dir, sizeof(char), CONST, arquivo);
		
		if( strcmp( ConstTab->fnome, TableName) == 0) {free(ConstTab);return;} //caso a tabela já exista
		fseek( arquivo, CONST*2, SEEK_CUR);
	}
	
	ConstTab->id ++;
	strcpy(ConstTab->lnome, TableName);
	puts(ConstTab->fnome);
	
	strcpy( ConstTab->fnome,GeneratePhysName(ConstTab.fnome));
	
	
	//strcpy(ConstTab->fnome, GeneratePhysName(ConstTab->fnome));
	
	
	
	fwrite(&ConstTab->id, sizeof(int), 1, arquivo);
	fwrite(ConstTab->lnome, sizeof(char), CONST, arquivo);
	fwrite(ConstTab->fnome, sizeof(char), CONST, arquivo);
	fwrite(ConstTab->dir, sizeof(char), CONST, arquivo);
	
	fclose(arquivo);
	free(ConstTab);
}



int main(){
	
	field *atributos; //o usuario cria um vetor para os atributos que ele ira criar;
	
	atributos = (field *) malloc(sizeof(field)* 2); //Neste caso tera dois atributos;
	
	strcpy(atributos[0].fName, "Nome");
	atributos[0].fType = 'S';
	atributos[0].fLenght = 10;
	
	strcpy(atributos[1].fName, "Idade");
	atributos[1].fType = 'I';
	atributos[1].fLenght = 2;  //Para Inteiros fLenght representa a quantidade de digitos
	
	createTable( "Cliente", atributos);
	return 0;
}
