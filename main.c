#include "Buffer.h"

int main(int argc, char *argv[])
{
	buffer bufferPool;
	field  fieldList;
	int choose = 0;
	bufferPool.countItems=0;
	while(choose != 2)
	{
		printf("Escolha uma opção\n0- Preencher o Buffer\n1- Listar Buffer\n2- Sair\n");
		scanf("%d",&choose);
		if(choose == 0)
		{
		//	| Tabelas disponíveis: fs_personagem, fs_status|
		//	| nome        | 
		//	| da tabela   |											   
			switch (fillBuffer(&bufferPool,&fieldList,"fs_personagem",argv[1])) {
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
			}
		}
		if(choose == 1)
		{
			if(showBuffer(&bufferPool) == BUFFER_VAZIO) printf("\nERRO - BUFFER VAZIO\n");
		}
	};
	return 0;

}
