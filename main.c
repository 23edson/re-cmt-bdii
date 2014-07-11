#include "Buffer.h"

int main(int argc, char *argv[])
{
	buffer *bufferPool=NULL;
	int choose = 0,i=1;
	while(choose != 2)
	{
		printf("Escolha uma opção\n0- Preencher o Buffer\n1- Listar Buffer\n2- Sair\n");
		scanf("%d",&choose);
		if(choose == 0)
		{
		//	| Tabelas disponíveis: fs_personagem, fs_status|
		//	| nome        | 
		//	| da tabela   |											   
			switch (fillBuffer(bufferPool,"Carro",i)) {
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
			switch(showBuffer(bufferPool,i)){
				case BUFFER_VAZIO:
				printf("\nERRO - BUFFER VAZIO\n");
				break;
				case INVALID_PAGE:
				printf("\nPAGINA NAO EXISTE\n");
				break;
			}
		};
		extractTupleFromBP(bufferPool,i);
	};
	free(bufferPool->fieldList);
	free(bufferPool);
	return 0;

}
