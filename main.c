#include "Buffer.h"

int main(){
	
	buffer *bufferPool = NULL; //Buffer NULO
	Element_t *myElem = NULL; //Variável que seŕa alocada posteriormente
	int erro; //Irá pegar os possíveis vindos das funções
	
	field *atributos; //o usuario faz uso de um vetor para os atributos que ele ira criar;
	Element_t *inserts; //Struct serve para inserções na tabela
	
	atributos = (field *) malloc(sizeof(field)*4); //Neste caso terá quatro atributos;
	inserts = (Element_t *) malloc(sizeof(Element_t) * 4);  //Cada posição do vetor representa um atributo(Mesmo número definido em atributos)
	
	
	strcpy(atributos[0].fName, "Model");
	atributos[0].fType = 'S';
	atributos[0].fLenght = 15;
	
	strcpy(atributos[1].fName, "Motor");
	atributos[1].fType = 'D';
	atributos[1].fLenght = 8; //Aqui o tamanho pode ser desconsidera, já que as funções gravam double com o seu tamanho já definido 
	
	strcpy(atributos[2].fName, "YearFab");
	atributos[2].fType = 'I';
	atributos[2].fLenght = 4; /*Para inteiros, tamanho representa a quantidade de dígitos do número. 
								ex: tamanho 4 (aceita números de 0 até 9999);*/

	strcpy(atributos[3].fName, "Class");
	atributos[3].fType = 'C';
	atributos[3].fLenght = 1; //O tamanho de caracter é 1byte, esse atributo não é precisamente necessário neste caso.
	
	
	/*Cria uma tabela chamada 'myCar', com os atributos no segundo parâmetros, e a quantidade de atributos
	 * que é definida no terceiro parâmetro */
	erro = createTable("myCar", atributos,4); 
	
	
	if(erro == OKAY){ //se a criação da tabela teve sucesso 
		
		inserts[0].type = String; 
		inserts[0].Str = malloc(sizeof(char)*5); /*O tamanho da String não interessa. 
		                                          A função usará o tamanho definido no metadados da tabela*/
		strcpy(inserts[0].Str, "HB20");
		inserts[1].type = Ndouble;
		inserts[1].Ddouble = malloc(sizeof(double));
		*inserts[1].Ddouble = 1.8;
		inserts[2].type = Nint;
		inserts[2].Dint = malloc(sizeof(int));
		*inserts[2].Dint = 2014;
		inserts[3].type = Caracter;
		inserts[3].Str= malloc(sizeof(char));
		*inserts[3].Str = 'B';
		
	
		/*Será inserido uma tupla na tabela 'myCar', com o segundo parâmetro sendo o conteúdo a ser inserido */
		erro = insertInto("myCar", inserts);
		
		
	}
	
	if(erro == OKAY){

		/*Se conseguiu inserir a tupla no arquivo em disco. Próximo passo é carregá-la para
		 * o buffer. Assim essa função se encarrega disso, passando o endereço de um ponteiro NULO ( Na primera chamada );
		 * segundo parâmetro é a tabela de qual será retirada a tupla. último parÂmetro é o número da tupla no disco,
		 * considera a ordem de inserção no arquivo.
		 * Para inserir mais de uma tupla, a função deve ser chamada n vezes, onde o terceiro parametro recebe o número
		 * de incremento.
		 * 
		 * Ex: while( i < nTuplas)
		 * 		fillBuffer(&bufferPool,"myCar", i)
		 * 		i++;
		 * */
		
	}
		erro = fillBuffer(&bufferPool,"myCar", 1);
		
		if(erro == OKAY){
			
		
			/*Sera extraído do buffer, a primeira tupla inserida, o terceiro parametro irá receber a quantidade de elementos retornados.
			 * Ou em caso de erro, recebe a flag Do erro */
			myElem = extractTupleFromBP(bufferPool, 1, &erro); 
			//puts(myElem[0].Str);
			/*Será estraído uma ou mais tupla do buffer, de modo que pertencem a mesma página.
			 * neste caso, página zero;
			 * 
				myElem = extractTuplesFromPage(bufferPool,0, &erro);
			*/
			int i = 0;
			if(myElem){
				while( i < erro){
					if(myElem[i].type == String)
						puts(myElem[i].Str);
					else if(myElem[i].type== Ndouble)
						printf("%f", *myElem[i].Ddouble);
					else if(myElem[i].type == Nint)
						printf("%d", *myElem[i].Dint);
					else
						printf("%c", *myElem[i].Str);
				i++;
				}
			}
		}
	
	return 0;

}

