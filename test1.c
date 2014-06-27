#include"Buffer.h"
#define CONST 30
#define BASE 10
#define CONST_MAX 94

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
 char *GeneratePhysName( char *last){
	//A finalidade dessa função é gerar um nome para o arquivo de dados da tabela x. Ex: se a o nome do ultimo arquivo foi : file_1.
	//Então o próximo será file_2.
	char c = 'a';
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
		c = last[i]; //Assuma que o número sempre estará após o underscore e antes do .dat
		if(c == '_'){
			isNumber = true;
		}
		else if(isNumber){
			number[shift] = c;
			shift++;
			if(last[i+1] == '.'){
				number[shift] = '\0';
				i = max;
			}
		}
		i++;
	}
	
	
	//number[shift] = '\0';
	long int num = strtol(number, NULL, BASE); //Conversao da string apontada por (last) em inteiro
	
	num++;
	
	char *fnome;
	
	fnome = ( char *) malloc(sizeof(char)* CONST);
	
	
    
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
    
    char *b;
    b = (char *)malloc(sizeof(char)*20);
    strcpy(b, "file_");
	strcat( b, fnome);
	strcat( b, ".dat");
	
	free(fnome);
	free(number);
	
	return b;
	
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
	//printf("Total: %ld\n", total);	
	rewind(arquivo); //coloca o ponteiro na posição zero
	int i = 0;
	
	criar *ConstTab = malloc(sizeof(criar));
	
	ConstTab->id = 0;
	strcpy(ConstTab->fnome,"file_0\0");
	strcpy(ConstTab->dir, "files/data/\0"); //Valores default
	
	for(;i < total;){ //Este laço percorre o arquivo e procura se já existe a tabela
		fread( &ConstTab->id, sizeof(int), 1, arquivo);	
		fread( ConstTab->lnome, sizeof(char), CONST, arquivo);
		fread( ConstTab->fnome, sizeof(char), CONST, arquivo); //puts(ConstTab->fnome);
		fread( ConstTab->dir, sizeof(char), CONST, arquivo);
		
		if( strcmp( ConstTab->lnome, TableName) == 0) {free(ConstTab);return;} //caso a tabela já exista
		//fseek( arquivo, CONST*2, SEEK_CUR);
		i += CONST_MAX ; //printf("val %d ", i);
	}
	//printf("Valor:%d", count);
	ConstTab->id ++;
	strcpy(ConstTab->lnome, TableName);
	//puts(ConstTab->fnome);

	char *getFromReturn = GeneratePhysName(ConstTab->fnome);
	strcpy( ConstTab->fnome, getFromReturn);//puts(ConstTab->fnome);
	
	fwrite(&ConstTab->id, sizeof(int), 1, arquivo);
	fwrite(ConstTab->lnome, sizeof(char), CONST, arquivo);
	fwrite(ConstTab->fnome, sizeof(char), CONST, arquivo);
	fwrite(ConstTab->dir, sizeof(char), CONST, arquivo);
	
	rewind(arquivo);
	fseek(arquivo, 0, SEEK_CUR);
	
	criar ConstTab1;
	
	fread(&ConstTab1.id, sizeof(int), 1 , arquivo);
	fread(ConstTab1.lnome, sizeof(char),CONST, arquivo);
	fread(ConstTab1.fnome, sizeof(char), CONST, arquivo);
	fread(ConstTab1.dir,sizeof(char), CONST, arquivo);
	
	printf("Id:%d\n", ConstTab1.id);
	puts(ConstTab1.lnome);
	puts(ConstTab1.fnome);
	puts(ConstTab1.dir);
	
	fclose(arquivo);
	free(ConstTab);
	free(getFromReturn);
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
