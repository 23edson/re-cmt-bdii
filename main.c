#include "Buffer.h"

int main(){
	
	buffer *bufferPool = NULL;
	Element_t *myElem = NULL;
	//int i = 1;
	int erro;
	
	field *atributos; //o usuario faz uso de um vetor para os atributos que ele ira criar;
	Element_t *inserts; //Struct serve para inserções na tabela
	//int contador = 0;
	
	
	atributos = (field *) malloc(sizeof(field)*4); //Neste caso terá quatro atributos;
	inserts = (Element_t *) malloc(sizeof(Element_t) * 4);  //Cada posição do vetor representa um atributo.
	
	
	strcpy(atributos[0].fName, "Model");
	atributos[0].fType = 'S';
	atributos[0].fLenght = 15;
	
	strcpy(atributos[1].fName, "Motor");
	atributos[1].fType = 'D';
	atributos[1].fLenght = 8; //Double desconsidera o tamanho 
	
	
	
	strcpy(atributos[2].fName, "YearFab");
	atributos[2].fType = 'I';
	atributos[2].fLenght = 4; //Para inteiros, tamanho representa a quantidade de dígitos do número. ex: tamanho 4 (aceita números de 0 até 9999);

	strcpy(atributos[3].fName, "Class");
	atributos[3].fType = 'C';
	atributos[3].fLenght = 1; //tipo caracter desconsidera o tamanho
	
	
	
	erro = createTable("myCar", atributos,4);
	
	//printf("erro: %d\n", erro);
	if(erro == OKAY){ //se a criação da tabela teve sucesso 
		
		inserts[0].type = String; 
		inserts[0].Str = malloc(sizeof(char)*5); //O tamanho da String não interessa. A função usará o tamanho definido no metadados da tabela
		strcpy(inserts[0].Str, "Impala");
		inserts[1].type = Ndouble;
		inserts[1].Ddouble = malloc(sizeof(double));
		*inserts[1].Ddouble = 3.6;
		inserts[2].type = Nint;
		inserts[2].Dint = malloc(sizeof(int));
		*inserts[2].Dint = 2014;
		inserts[3].type = Caracter;
		inserts[3].Str= malloc(sizeof(char));
		*inserts[3].Str = 'B';
		
		//puts("aqu");
		//printf("c:%c", *inserts[3].Str);
		
		erro = insertInto("myCar", inserts);
		//printf("%d\n\n",erro);
		
	}
	/*struct meta{
		int id;
		char atnome[40];
		char tipo;
		int tam;
	};
	typedef struct meta META;
	
	META di[4];*/
	
	
	//Attribute in;

	/*FILE *m = fopen("files/data/file_1.dat", "r");if(!m) return 2;
	int g;
	double v;
	char b;
	fseek( m, 0, SEEK_SET);
	
	char *t;
	t =	malloc(sizeof(char)*15);
	fread( t, sizeof(char), 15,m); puts(t);
	fread( &v, sizeof(double), 1,m); printf("vd %f\n", v);
	
	
	
	
	//fread( &v, sizeof(double), 1,m); printf("m %f\n", v);
	fread( &g, sizeof(int), 1,m); printf("vt %d\n", g);
	fread( &b, sizeof(char), 1,m);printf(" f:%c ", b);
	*/
	


								   
		erro = fillBuffer(&bufferPool,"myCar", 1);
		
	
	
	//
		myElem = extractTupleFromBP(bufferPool,1);
		//printf(" %p ", myElem);
	//puts("aq//ui");	
		if(!myElem)
			puts("aq");
		printf(" %s ",  myElem[0].Str);
		printf(" %f ", *myElem[1].Ddouble);
		printf(" %d ", *myElem[2].Dint);
		printf(" %c ", *myElem[3].Str);
	//bufferPool->bp[0]->data[];*/
	
	/*FILE *p = fopen("files/data/file_1.dat", "r");
	
	char v[15];
	int v1;
	double v2;
	char v3;
	fseek(p, 112, SEEK_SET);
	fread(v, sizeof(char), 15, p);
	fread(&v2, sizeof(double), 1, p);
	fread(&v1, sizeof(int), 1, p);
	fread(&v3, sizeof(char), 1, p);
	
	puts(v);
	printf("%f", v2);
	printf(" %d ", v1);
	printf(" %c ", v3);*/
	
	//if(showBuffer(bufferPool));
	puts("\n");
	bufferFree(bufferPool);
		
	return 0;

}

