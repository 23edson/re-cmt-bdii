#include "Buffer.h"

int main()
{
	buffer *bufferPool = NULL;
	//field  fieldList;
	//tuple *myTuple = NULL;
	int i = 1;
	int erro;
	while(i <= 5)	{							   
	erro = fillBuffer(&bufferPool,"Carro",i);
	
	i++;
	}
	puts("aqui");
	//printf("erro : %d", erro);
	extractTupleFromBP(bufferPool,1);
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
		
	return 0;

}
