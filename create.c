#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int HeyCreateTable(char *comando){
	
	
	char* a2[] = { "create", "table" };
	char* types[] = { "string", "int", "double", "char"};
	char* att[];
	char* tipoAtt[];
	int i,j, = 0;
	int control = 0;
	int cWord = 0;
	int initAtt = 0;
	int parenteses = 0;
	int tam = strlen (comando);
	char c;
	char word[50];
	word[0] ='\0';
	while(control < tam){
		c = comando[control];
		
		
			
		if( c != ' '){
			if( c == '('){
				initAtt = 1;
				parenteses++;
				while( c != ')' && parenteses > 0){
					 control++;
					 c = comando[control];
					 if(c != ' '){
						 att[j] = c;
						 if(comando[control + 1] == ' '){
							 control++;
							 j++;
							 
					 }
			}
			else{
				word[cWord] = c;
				if(comando[control + 1] == ' '){
					word[cWord + 1] = '\0';
					if( strcmp(word, a2[i]) != 0){
						return -1;
					}else {word[0] = '\0'; cWord = -1; i++;}
				}
				cWord++; 
			}
		}
		control++;
	}
	return 1;
}
int main(){

	char *teste = "create table cliente( nome string(10), idade int);"; 
	
	int i = HeyCreateTable(teste);
	
	if( i < 0) printf("erro"); 

	return 0;
	
}
