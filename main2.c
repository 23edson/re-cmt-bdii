#include "Buffer.h"

int main(int argc, char *argv[])
{
	buffer *bufferPool=NULL;
	field *fieldAdd=NULL;
	char *nome=NULL;
	nome=(char*)malloc(sizeof(char)*TNAME_LENGHT);
	if(!nome)
		printf("\nSem Memoria\n");
		
	int choose = 0,i=1;
	while(choose != 2)
	{
		printf("Escolha uma opção\n0- Preencher o Buffer\n1- Listar Buffer\n2- Sair\n4- criar tabela");
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
			
		}
		else if(choose == 3){
			extractTupleFromBP(bufferPool,i);
		}
		else if(choose == 4){
			int nfields=0,i;
			printf("\nDigite o nome da tabela: ");
			fgets(nome,TNAME_LENGHT-1,stdin);
			printf("\nQuantos Atributos terão: ");
			scanf("%d",&nfields);
			fieldAdd=(field*)malloc(sizeof(field)*nfields);
			if(!fieldAdd){
				printf("\nSem Memoria\n");
				free(nome);
				if(bufferPool){
					free(bufferPool->fieldList);
					free(bufferPool);
				}
				return 0;
			}
			for(i=0;i<nfields;i++){
				printf("\nNome do Atributo: ");
				fgets(fieldAdd[i].fName,TNAME_LENGHT-1,stdin);
				printf("\nTipo do Atributo S, I, D ou C: ");
				scanf("%c",&fieldAdd[i].fType);
				printf("\nTamanho do Atributo (numero): ");
				scanf("%d",&fieldAdd[i].fLenght);
			};
			
			free(fieldAdd);
		};
	};
	
	if(bufferPool){
		free(bufferPool->fieldList);
		free(bufferPool);
	}
	free(nome);
	return 0;

}
