#include "Buffer.h"
int create(char *a1){//dicionario
		FILE *fpd=NULL;
	if((fpd=fopen("dicionario.dat","r"))==NULL)
		printf("não foi possivel abrir dicionario");
	FILE *fp=NULL;
	if((fp=fopen("dictionary.dat","w"))==NULL)
		printf("não foi possivel criar novo arquivo");
	char* a2 =NULL;
	a2=(char*)malloc(sizeof(char)*50);
	fwrite(a1,sizeof(char),12,fp);
	
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
int read(){//dicionario
	char *a2=NULL;
	FILE *fp=NULL;
	if((fp=fopen("dictionary.dat","r"))==NULL)
		printf("não foi possivel criar novo arquivo");
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
int main(){
	create("colunas.dat");
	read();
	return 0;
}
