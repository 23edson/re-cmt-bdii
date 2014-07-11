#include "Buffer.h"

int main(int argc, char *argv[])
{
	buffer *bufferPool=NULL;
	int choose = 0;
	while(choose != 2)
	{
		printf("Escolha uma opção\n0- Preencher o Buffer\n1- Listar Buffer\n2- Sair\n");
		scanf("%d",&choose);
		if(choose == 0)
		{
		//	| Tabelas disponíveis: fs_personagem, fs_status|
		//	| nome        | 
		//	| da tabela   |											   
			switch (fillBuffer(bufferPool,"Carro",1)) {
		//	| Arquivo de dicionário passado como parâmetro |
				case ERRO_ARQUIVO: 
					printf("\nERRO NA LEITURA DO ARQUIVO OU ARQUIVO INEXISTENTE\n");
					break;
				case TABELA_NOTFOUND:
					printf("\nTABELA NÃO ENCONTRADA OU INEXISTENTE\n");
					break;
				case BUFFER_PREENCHIDO: 
					printf("\nBUFFER PREENCHIDO\n");
					break;
				case OUT_MEMORIA:
					printf("\nSEM MEMORIA DISPONIVEL\n");
					break;
			}
		}
		else if(choose == 1)
		{
			switch(showBuffer(bufferPool,0)){
				case BUFFER_VAZIO:
				printf("\nERRO - BUFFER VAZIO\n");
				break;
				case INVALID_PAGE:
				printf("\nPAGINA NAO EXISTE\n");
				break;
			}
		};
	};
	free(bufferPool);

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
