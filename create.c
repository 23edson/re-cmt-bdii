#include "Buffer.h"
int searchId(FILE *fp,int total){
	
	
}
int createDic(char *a1,field *Add){//dicionario a partir do dicionario do grupo anterior
	FILE *fpd=NULL;
	if((fpd=fopen("files/fs_tabela.dat","r+"))==NULL)
		return ERRO_ARQUIVO;
	FILE *fp=NULL;
	if((fp=fopen("files/fs_coluna.dat","r+"))==NULL)
	{
		fclose(fpd);
		return ERRO_ARQUIVO;
	}
	char* a2 =NULL;
	a2=(char*)malloc(sizeof(char)*50);
	if(!a2){
		fclose(fpd);
		fclose(fp);
		return ERRO_ARQUIVO;
	};
	int i=0,total=0;
	fseek(fdp,0,SEEK_END);
	total=ftell(fpd);
	rewind(fpd);
	i=searchId(fpd,total);
	
	fwrite(a1,sizeof(char),TNAME_LENGHT,fp);
	
	int i=0;
	for(;i<50&&feof(fpd)==0;i++){
		fread(&a2[i],sizeof(char),1,fpd);
		if(a2[i]=='\0'){
			if(a2[i-4]!='.'){
				fwrite(a2,sizeof(char),strlen(a2)+1,fp);
				
				i=(-1);
				memset(a2,0,strlen(a2)+4);
			}
			else{
				i=(-1);
				memset(a2,0,strlen(a2)+4);	
			}
		}
	}	
	fclose(fp);
	fclose(fpd);
	free(a2);
	return 0;
}
int readDic(){//dicionario
	char *a2=NULL;
	FILE *fp=NULL;
	if((fp=fopen("dictionary.dat","r"))==NULL)
		return ERRO_ARQUIVO;
	a2=(char*)malloc(sizeof(char)*50);
	int i=0;
	for(;i<50&&feof(fp)==0;i++){
		fread(&a2[i],sizeof(char),1,fp);
		if(a2[i]=='\0'){	
			printf("%s\n",&a2[0]);
			i=(-1);
			memset(a2,0,50);	
		}
	}	
	fclose(fp);
	free(a2);
	return 0;
}
int createCol(char *a1){
	FILE *fp=NULL,*fpd=NULL;
	char *a2=NULL;
	if((fpd=fopen("dictionary.dat","r"))==NULL)
		return ERRO_ARQUIVO;
	a2=(char*)malloc(sizeof(char)*50);
	
	fclose(fpd);
	free(a2);
	return 0;
	
}

int main(){
	createDic("colunas.dat");//cria o dictionary.dat
	readDic();
	return 0;
}
