
/**	
* Este trabalho utiliza a licença GNU General Public License.
* gcc (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3
* Copyright (C) 2011 Free Software Foundation, Inc.
* 
**/
#include "Buffer.h"

/**
 * 
 * @defines constantes das estruturas
 **/  

#define CONST 30  
#define BASE 10 
#define CONST_MAX 94  
#define CONST_META 29


/**
 * @struct Ctabela representa uma tabela em disco,
 * onde id é número inteiro maior que zero;
 * lnome é o nome lógico, ex : cliente;
 * fnome é o nome físico, ex: file_1.dat
 * dir é a localização em disco, ex: data/files/file_1.dat
 **/
struct Ctabela{
		int id;
		char lnome[CONST]; 
		char fnome[CONST];
		char dir[CONST];
};
typedef struct Ctabela criar;

//Union para o tipo inteiro
union c_int{
	int  num;
	char cnum[sizeof(int)];
};

//Union para o tipo double
union c_double{
	double numd;
	char  cnumd[sizeof(double)];
};
	
/**
 * Inicializa o buffer com os valores default.
 * @param buffer *bPool é um ponteiro do tipo buffer não nulo.
 * @struct buffer está definido no arquivo Buffer.h; 
 * Constante BP_SIZE e BP_PAGES representam respectivamente o tamanho
 * e a quantidade de páginas no bufferPool.
 * @return não possui retorno.
 **/
void initBuffer(buffer *bPool){
	int i;
	
	for(i = 0; i < BP_PAGES; i++){
		bPool->bp[i].diskSeek = 0;
		bPool->bp[i].data = malloc(sizeof(char)*BP_SIZE);
		bPool->bp[i].fieldList = NULL;
		bPool->bp[i].fieldCount = 0;
		bPool->bp[i].countTuples = 0;
	}
	bPool->countItems = 0;
	bPool->nextPageAvaliable = 0;
	
}

/**
 * Copia uma sequência de caracteres a partir de uma posição i.
 * @param char *tupla é a string de destino;
 * @param char *vd é a string  de origem;
 * @param int init, posição inicial em *tupla;
 * @param int tam, quantidade de bytes a serem copiados.
 * 
 * @return não possui retorno essa função.
 **/
void cpyvar(char *tupla,char *vd,int init,int tam){
	int i=init,j=0;
	
	for (;i<tam+init;i++)
		tupla[i]=vd[j++];
}

/**
 * Soma os tamanhos dos atributos conforme os seus tipos.
 * 
 * @struct field está definida em Buffer.h
 * @param field *fieldList - lista de atributos já definidos;
 * @param int fieldCount - Quantidade de atributos de fieldList;
 * @return int - Número da soma em bytes dos atributos.
 **/
int tupleTam( field *fieldList, int fieldCount){
	int i;
	int sum =0;
	for( i = 0; i < fieldCount; i++){
		if(fieldList[i].fType == 'I')
			sum += sizeof(int);
		else if(fieldList[i].fType == 'D')
			sum += sizeof(double);
		else if (fieldList[i].fType == 'C')
			sum += sizeof(char);
		else
			sum += fieldList[i].fLenght;
		
	}
	return sum;
}

/**
 * Procura o número da página e a posição inicial de uma determinada tupla na mesma.
 * Função auxiliar de extractTupleFromBP. 
 * @param buffer *bufferPool - buffer não nulo, aonde estão as tuplas a serem examinadas;
 * @param int tupleNumber - Indica o número da tupla procurada no buffer;
 * @param int *pagina - Após a localização da tupla e da página, essa variável recebe o número da página;
 * @return int - Posição inicial da tupla no buffer.  
 **/
int getTuplePosition(buffer *bufferPool, int tupleNumber, int *pagina){
	
	int total = bufferPool->countItems;
	
	if(bufferPool->countItems == 0)
		return EMPTY_BUFFER;
	
	int i = 1;
	int j = 1;
	int page = 0;
	int flag = 1;
	int position = 0;
	int contador = 1;
	while(i <= total){
		while(j <= bufferPool->bp[page].countTuples){
			
			if(tupleNumber == contador){
				flag = 0;
				
				position = (tupleTam(bufferPool->bp[page].fieldList, bufferPool->bp[page].fieldCount)*(j -1));
				break;
			}
			j++;
			contador++;
		}
		if(flag==0){
			*pagina = page;
			return position;
		}
			
		else{
			
			i += j-1;
			j = 1;
			page++;
		}
		
	}
	
	return ABORT;
}

/**
 * Copia uma sequência de caracteres de acordo com os tamanhos específicados;
 * Função semelhante à cpyvar, entretando nessa os dados são gravados sempre 
 * a partir do índice zero. 
 * 
 * @param char *destino - string que armazenará a sequência copiada;
 * @param char *origem - A fonte para a cópia;
 * @param int initOrigem - A partir daqui serão buscados os caracteres no string origem;
 * @param int destTam - Quantidade de caracteres;
 * 
 **/
void copia_string( char *destino,char *origem, int initOrigem, int destTam){
	
	int i = initOrigem;
	int j = 0;
	
	for( ; i < initOrigem + destTam;i++)
		destino[j++] = origem[i];
	
}

/**
 *Extrai uma tupla específica do buffer, considerando a ordem de inserção. 
 * 
 * Funcionamento: O número da tupla a ser removida deve ser maior que 0 e menor
 * que o número de elementos no buffer. Após obter o número da página e a posição
 * inicial da tupla, a função aloca uma estrutura conforme os metadados corretos
 * dela. Assim é gravado e retornado ao chamador.
 * 
 @struct Element_t - está definida em Buffer.h
 * @param buffer *bufferPool - não nulo;
 * @param int tupleNumber - Número da tupla a ser extraída.
 * @return Element_t - Após pegar uma tupla do buffer, é alocado uma struct
 * assim, ela é devolvida com seus devidos elementos.
 * 
 **/
Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber, int *quantidade){
	int i;
	int page;
	
	Element_t *myTuple = NULL;
	int sum = 0;
	
	
	if(!bufferPool){
		*quantidade = ABORT;
	}
		return NULL;
	if(tupleNumber < 1 || tupleNumber > bufferPool->countItems){
		*quantidade = TUPLE_NOT_FOUND;
		return NULL;
	}
	
	
	
	int position = getTuplePosition(bufferPool, tupleNumber, &page);
	
	myTuple = (Element_t *)malloc(sizeof(Element_t) * bufferPool->bp[page].fieldCount);
	if(!myTuple || position == EMPTY_BUFFER){
		*quantidade = EMPTY_BUFFER;
		return NULL;
	}
	
	sum = position;
	for(i = 0; i < bufferPool->bp[page].fieldCount; i ++){
		if(bufferPool->bp[page].fieldList[i].fType == 'S'){
			myTuple[i].type = String;
			myTuple[i].Str = (char *) malloc(sizeof(char)*bufferPool->bp[page].fieldList[i].fLenght);
			copia_string(myTuple[i].Str, bufferPool->bp[page].data, sum, bufferPool->bp[page].fieldList[i].fLenght);
			sum += bufferPool->bp[page].fieldList[i].fLenght;
			
		}
		else if(bufferPool->bp[page].fieldList[i].fType == 'I'){
			myTuple[i].type = Nint;
			myTuple[i].Dint = (int *)&bufferPool->bp[page].data[sum];
			sum += sizeof(int);
		}
		else if(bufferPool->bp[page].fieldList[i].fType == 'D'){
			myTuple[i].type = Ndouble;
			myTuple[i].Ddouble = (double *)&bufferPool->bp[page].data[sum];
			sum += sizeof(double);
		}
		else{
			myTuple[i].type = Caracter;
			myTuple[i].Str = malloc(sizeof(char));
			*myTuple[i].Str =bufferPool->bp[page].data[sum];
			sum += sizeof(char);
		}	
	}
	*quantidade = bufferPool->bp[page].fieldCount;
	return myTuple;
}

/**
 * Extrai uma ou mais tuplas do buffer
 * 
 * Funcionamento: conforme o número passado pelo @param page procura nela
 * todas as tuplas e retorna em um vetor
 * 
 **/
Element_t *extractTuplesFromPage(buffer *bPool, int page, int *quantidade){
	
	if(page < 0 || page >= BP_PAGES){
		*quantidade = VALOR_INVALIDO;
		return NULL;
	}
	if(bPool->countItems == 0){
		*quantidade = EMPTY_BUFFER;
		return NULL;
	}
	if(!bPool){
		*quantidade = ABORT;
		return NULL;
	}
	Element_t *myTuples = NULL;
	int i;
	int j = 0;
	int tuple = 0;
	int sum = 0;
	int total = bPool->bp[page].fieldCount * bPool->bp[page].countTuples;
	myTuples = malloc(sizeof(Element_t)*total);
	
	while(j < total){
		for( i = 0; i < bPool->bp[page].fieldCount;i++, tuple++){
			if(bPool->bp[page].fieldList[i].fType == 'S'){
				myTuples[tuple].type = String;
				myTuples[tuple].Str = (char *) malloc(sizeof(char)*bPool->bp[page].fieldList[i].fLenght);
				copia_string(myTuples[tuple].Str, bPool->bp[page].data, sum, bPool->bp[page].fieldList[i].fLenght);
				sum += bPool->bp[page].fieldList[i].fLenght;
			
			}
			else if(bPool->bp[page].fieldList[i].fType == 'I'){
				myTuples[tuple].type = Nint;
				myTuples[tuple].Dint = (int *)&bPool->bp[page].data[sum];
				sum += sizeof(int);
			}
			else if(bPool->bp[page].fieldList[i].fType == 'D'){
				myTuples[tuple].type = Ndouble;
				myTuples[tuple].Ddouble = (double *)&bPool->bp[page].data[sum];
				sum += sizeof(double);
			}
			else{
				myTuples[tuple].type = Caracter;
				myTuples[tuple].Str = malloc(sizeof(char));
				*myTuples[tuple].Str =bPool->bp[page].data[sum];
				sum += sizeof(char);
			}	
		
		}
		j+= bPool->bp[page].fieldCount;
	}
	*quantidade = total;
	return myTuples;
}

/**
 * Política de substituição LRU
 * 
 * Aplica as funcionalidades necessárias para a substituição.
 * 
 * 
 * */
void applyReplacementPolicies(buffer *bPool){
	//Aqui é escolhido a página do buffer que tem o menor pinCount para ser substituida
	int i,j=0, lower = bPool->bp[0].pinCount;
	bufferPage bPage = bPool->bp[0];
	for(i = 0; i < BP_PAGES; i++){
		if(bPool->bp[i].pinCount < lower){
			bPage = bPool->bp[j];
			lower = bPool->bp[j].pinCount;
			j=i;
		}
	}
	//A página encontrada fica armazenada em bPage
	if(bPage.rewriteBit == 1){
		//Aqui deve ser colocado o código para regravação do arquivo.
		Element_t *cr=NULL;
		int erro;
		cr=extractTupleFromBP(bPool,j,&erro);
		
		free(cr);
	}
	else{
		bPool->bp[i].diskSeek = -1;
		bPool->nextPageAvaliable = i;
	}
}

/**
 * Função auxiliar de bufferInsert. Procura a página
 * que contém o id específicado.
 * 
 * @param buffer *bPool - Fonte, não nula
 * @param int id - número de identicação de uma tabela;
 * @param int tamanho - Quantidade de metadados no buffer
 * 
 **/
int procura_meta(buffer *bPool, int id, int tamanho){
	int i;
	
	for( i = 0; i< tamanho; i++){
		if(bPool->bp[i].idNumber == id)
			return i;
	}
	return -1;
}

/**
 * Faz a inserção de uma tupla no buffer, conforme os itens específicados pelos parâmetros;
 * Organização: Uma página x armazena tuplas apenas da mesma tabela. a Constante BP_SIZE define
 * o tamanho da página, assim pode-se altera-lá para obter um espaço maior para cada tabela no buffer.
 * 
 * Funcionamento : Se não possui mais páginas livres, uma delas devem ser substituída, aqui está definida
 * a mais antiga (LRU). Deste modo, uma tabela é liberada do buffer.
 * É feita a procura no buffer, em cada inserção, assim para garantir que cada página tenha tuplas de
 * tabelas diferentes.
 * 
 * @param buffer *bPool - buffer devidamente alocado;
 * @param char *tupla - Sequência de caracteres que serão adicionados ao bufferPool;
 * @param field *fieldList - Metadados da tabela e da tupla a ser inserida;
 * @param int fieldCount - números de atributos de tabela;
 * @param int id - identicador da tabela, este id é recuperado do disco;
 * 
 * @return int - Flag de status, pode retornar OKAY ( número que representa sucesso). OU
 * ABORT (representa um erro genérico, não específico e/ou não tratado).
 **/
int bufferInsert(buffer *bPool,char *tupla,int tupleLenght, field *fieldList, int fieldCount, int id){
	
	if(!bPool)
		return ABORT;
	//Primeiro verifica se o buffer está cheio, caso esteja ele aplica as políticas de troca
	if(bPool->countItems >= BP_PAGES){
		applyReplacementPolicies(bPool);
	}
	
	int teste = procura_meta(bPool, id, bPool->countMeta);
	
	if(teste < 0){
		bPool->bp[bPool->nextPageAvaliable].fieldList = fieldList;
		bPool->bp[bPool->nextPageAvaliable].fieldCount = fieldCount;
	    bPool->bp[bPool->nextPageAvaliable].idNumber = id;
		teste = bPool -> nextPageAvaliable;
		bPool->countMeta++;
		bPool->nextPageAvaliable++;
		
	}
	//É inserido na próxima página disponível
	
	if( BP_SIZE - bPool->bp[teste].diskSeek > tupleLenght){
		cpyvar(bPool->bp[teste].data, tupla, bPool->bp[teste].diskSeek, tupleLenght);
		
		bPool->bp[teste].diskSeek += tupleLenght;
		bPool->bp[teste].pinCount = 0;
		bPool->bp[teste].rewriteBit = 0;
		bPool->bp[teste].countTuples++;
		bPool->countItems ++;
	
		return OKAY;
	}
	return ABORT;
}

/**
 *  Faz a contagem de números de atributos de uma tabela.
 *  Funcionamento: a partir de uma número de id específicado, 
 *  a função percorre o arquivo fs_coluna.dat, movendo o ponteiro
 *  em relação aos tamanhos de cada estrutura, e fazendo a contagem
 *  de números de atributos de uma tabela x que possui o id da tabela
 *  requisitada.
 * 
 * 	@param int init - Aonde começa os metadados da tabela no arquivo;
 *  @param FILE *metadados - Arquivo fs_coluna.dat
 * 	@param int total - Representa o tamanho em bytes do arquivo.
 *  @param criar *myTable - struct com a tabela, recuperado do disco anteriormente.
 *  @return int - Devolve o número contados de atributos
 **/
int counter( int init, FILE *metadados, int total, criar *myTable ){
	
	int copiar = myTable->id;
	int AttCount = 0;
	int i = init; 
	
	fseek( metadados, init , SEEK_SET);
	
	while(i < total){ 
		fread( &copiar, sizeof(int), 1, metadados);
		if(myTable->id != copiar)
			break;
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
		
	}
	return AttCount;
}

/**
 * Conta no arquivo o número de tuplas, tratando com um índice para tupla.
 * Funcionamento: Conforme o tamanho do arquivo, muda o ponteiro no arquivo
 * em relação ao tamanho da tupla. Assim, Quando a contagem é igual a tupla
 * requisitada, é devolvido sua posição.
 * 
 * @param FILE *arquivo - Arquivo de dados a serem lidos;
 * @param int position - Número que representa uma tupla, considerando a ordem de inserção;
 * @param int tamTuple - Tamanho de uma tupla.
 * @return int - TUPLE_NOT_FOUND ( Quando não encontra em disco ou o número da tupla não é válido)
 * e OKAY ftell(arquivo) : Posição da tupla no arquivo.
 * 
 * */
int getTupleNumber(FILE *arquivo, int position, int tamTuple){
	if(position < 1)
		return TUPLE_NOT_FOUND;
	
	int analise = 0;
	int endLoop = 0;
	fseek(arquivo, 0, SEEK_END);
	long tam = ftell(arquivo);
	rewind(arquivo);
	
	while(endLoop < tam){
		fseek(arquivo, tamTuple, SEEK_CUR);
		analise ++;
		if(position == analise){
			fseek(arquivo, -tamTuple, SEEK_CUR);
			return ftell(arquivo);
		}
		endLoop += tamTuple;
	}
	return TUPLE_NOT_FOUND;
	
}

int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador){
	
	FILE *tabela = fopen("files/fs_tabela.dat","r");
	if(tabela == NULL) return FILE_NOT_FOUND;
	
	
	//Abertura dos arquivos de dados e de metadados
	char *name = NULL; //Name representa o nome da tabela
	if((name=(char*)malloc(50))==NULL){
		fclose(tabela);
		return OUT_MEMORIA;
	}
	
	char *caminho=NULL; 
	if((caminho=(char*)malloc(50))==NULL){
		fclose(tabela);
		free(name);
		return OUT_MEMORIA;
	}
	
	criar *biblio=NULL;
	biblio=(criar*)malloc(sizeof(criar));
	if(biblio==NULL){
		fclose(tabela);
		free(name);
		free(caminho);
		return OUT_MEMORIA;
	}
	
	int i = 0, achou=0;
	int endLoop = 0;
	
	fseek(tabela,0,SEEK_END); //Coloca o ponteiro do arquivo (tabela) no final.
	long total = ftell(tabela); //Retorna o posição atual do ponteiro no arquivo.
	rewind(tabela); //Coloca o ponteiro no início do arquivo.
	
	for(;endLoop < total; endLoop += CONST_MAX ){
		fread(&biblio->id, sizeof(int), 1, tabela);
		fread(biblio->lnome,sizeof(char),CONST,tabela);
		fread(biblio->fnome,sizeof(char),CONST,tabela);
		fread(biblio->dir,sizeof(char),CONST,tabela);
		if((strcmp(biblio->lnome,nomeTabela))==0){	
			achou=1;
			strcpy(caminho,biblio->dir);
			strcat(caminho,biblio->fnome);
			break; // sai do loop pois encontrou a tabela
		}
		
	}
	if (achou == 0){ 
		fclose(tabela);
		free(name);
		free(caminho);
		free(biblio);
		return TABLE_NOT_FOUND;
		 // fecha arquivo do dicionario
	}
	
	FILE *meta=fopen("files/fs_coluna.dat","r");// abre meta-dados
	if(meta == NULL) return FILE_META_NOT_FOUND;

	FILE *arquivo = fopen(caminho,"r"); // abre arquivo de dados
	if(arquivo == NULL) return FILE_DATA_NOT_FOUND;

	
	int copiar = 0;
	
	fseek(meta,0,SEEK_END);
	total = ftell(meta);
	long pos = -1;
	rewind(meta); 
	
	 
	while(i < total){ //Procura a posicao inicial do metadados
		
		fread( &copiar, sizeof(int), 1, meta);
		if(copiar == biblio->id){
			pos = ftell(meta) - sizeof(int);
			break;
		}
			
		else{
			fseek( meta, CONST_META - sizeof(int) , SEEK_CUR);
			i +=CONST_META;
		}
	}
	
	if( pos == -1)
		return TABLE_NOT_FOUND; //Table Not Found
	
	int fieldCount = counter( pos, meta, total, biblio); //Números de atributos da tabela
	
	fseek(meta, pos, SEEK_SET); //Seta o ponteiro para o início dos atributos da tabela 

	
	field *fieldList = NULL;
	if(( fieldList = malloc(sizeof(field) * fieldCount))==NULL){
			return OUT_MEMORIA;
	}; // aloca lista com o numero de campos que tem a tabela

	
	if(!*bufferPool){ //recebe ponteiro null para saber quando deve ser inicializado
		*bufferPool = (buffer *)malloc(sizeof(buffer));
		
		if(!*bufferPool)
			return OUT_MEMORIA;
		
		initBuffer(*bufferPool);
	}
	
		
	
	i = 0;
	
	int tupleLenght = 0;
	
	
    
	for(i = 0; i < fieldCount; i++){
		fseek( meta, sizeof(int), SEEK_CUR);
		fread(&fieldList[i].fName,sizeof(char),TNAME_LENGHT, meta);
		fread(&fieldList[i].fType,sizeof(char),1,meta);
		fread(&fieldList[i].fLenght,sizeof(int),1,meta);
		//Vai montando o tamanho da tupla com base nos tamanhos dos campos encontrados
		if(fieldList[i].fType == 'D')
			tupleLenght += sizeof(double);
		else if(fieldList[i].fType == 'I') 
			tupleLenght += sizeof(int);
			
		else if(fieldList[i].fType == 'C')
			tupleLenght += sizeof(char);
		else
			tupleLenght += fieldList[i].fLenght;
	}
	
	
	
	contador = getTupleNumber(arquivo,contador,tupleLenght);
	if(contador == TUPLE_NOT_FOUND)
		return contador;
	//até aqui vamos deixar no arquivo colunas.dat
	//Cria os campos temporários para a montagem da tupla
	fclose(meta);
	int *tInt = NULL;
	if((tInt=malloc(sizeof(int)))==NULL){
		return OUT_MEMORIA;
	}
	double *tDouble = NULL;
	if((tDouble=malloc(sizeof(double)))==NULL){
		return OUT_MEMORIA;
	}
	char *tChar = NULL;
	char *tTuple = NULL;
	if((tTuple=malloc(sizeof(char)*tupleLenght))==NULL){
		return OUT_MEMORIA;
	}
	
	tTuple[0] = '\0';
	union c_double vdouble;
	union c_int vint;
	
	int thePointer = 0;
	
	fseek(arquivo, contador, SEEK_SET);
	//Começa a leitura dos dados.
	
		//Cada tupla lida é inserida no buffer
		for(i = 0; i < fieldCount; i++){
			//Cada campo de tupla é checado seu tipo e adicionado corretamente na tupla temporária *tTuple
			if(fieldList[i].fType == 'I'){
				//Inteiro
				fread(tInt,sizeof(int),1,arquivo);
				vint.num = *tInt;
				cpyvar(tTuple,vint.cnum,thePointer,sizeof(int));
				thePointer += sizeof(int);
			}
			else if(fieldList[i].fType == 'D'){
				//Double
				fread(tDouble,sizeof(double),1,arquivo); 
				vdouble.numd = *tDouble; 
				cpyvar(tTuple,vdouble.cnumd,thePointer,sizeof(double));
				thePointer += sizeof(double);
			}
			else if(fieldList[i].fType == 'S'){
				
				//String
				if((tChar = (char *)malloc(sizeof(char) * fieldList[i].fLenght))==NULL){
					return OUT_MEMORIA;
				}; 
				fread(tChar,sizeof(char),fieldList[i].fLenght,arquivo);
				
				cpyvar( tTuple, tChar, thePointer, fieldList[i].fLenght);
				thePointer =+ fieldList[i].fLenght;
				free(tChar);
			}
			else if(fieldList[i].fType == 'C'){
				//Caracter
				char c;
				fread(&c,sizeof(char),1,arquivo);
				tTuple[thePointer++] = c;
			}
		}
		
	int getErro = bufferInsert(*bufferPool,tTuple,tupleLenght,fieldList,fieldCount,biblio->id);
			
	
	//Fecha o arquivo de dados e libera a memória
	fclose(arquivo);
	free(name);
	free(caminho);
	free(tInt);
	free(tDouble);
	free(tTuple);
	return getErro;

}

/**
 * Procura por uma tabela no arquivo fs_tabela.dat
 * Funcionamento: é lido o tamanho de uma tabela definido
 * pela constante : CONST_MAX. Depois é comparado o nome lógico
 * recuperado do arquivo, com o nome requisitado.
 * 
 * @param FILE *arquivo - Arquivo fs_tabela.dat
 * @param char *Tabela - nome da tabela a ser procurada
 * @param criar **myTable - Estrutura para armazenar a tabela recuperada
 * @return int - FILE_NOT_FOUND( arquivo fs_tabela.dat não foi achado no disco),
 * TABLE_NOT_FOUND ( Tabela não existe no arquivo), OKAY ( Êxito na procura);
 * 
 **/ 
int searchTable(FILE *arquivo, char *Tabela, criar **myTable){
	
	if(!arquivo)
		return FILE_DATA_NOT_FOUND;
	int contador = 0;
	criar *table = (criar *)malloc(sizeof(criar));
	if(!table)
		return 0;
	
	fseek( arquivo, 0, SEEK_END);
	int total = ftell(arquivo);
	rewind(arquivo);
	
	for(;contador < total;){
		fread( &(*myTable)->id, sizeof(int), 1, arquivo);	
		fread( (*myTable)->lnome, sizeof(char), CONST, arquivo);
		fread( (*myTable)->fnome, sizeof(char), CONST, arquivo);
		fread( (*myTable)->dir, sizeof(char), CONST, arquivo);
		
		if( strcmp( (*myTable)->lnome, Tabela) == 0) {//caso encontrou a tabela
			free(table);
			fclose(arquivo);
			return OKAY;
		} 
		contador += CONST_MAX ;
	}
	free(table);
	fclose(arquivo);
	return TABLE_NOT_FOUND; //erro table not found
}
		
/**
 * Seu objetivo é verificar se um número está dentro de um padrão definido,
 * ou seja, se dizemos que um inteiro pode ser apenas de 2 dígitos, então
 * o limite deve ser de ( 0 até 99).
 * 
 * Funcionamento: o número é comparado se é menor com alguns múltiplo de 10, ou seja,
 * neste caso, se é menor que (10,100,1000,10000,...);
 * Deste modo, caso seja menor, que algum deles, e seu contador interno for igual ao
 * número de dígitos. Então esse número respeita o limite.
 * 
 * 
 * @param int numero - Valor a ser comparado;
 * @param int count - Quantidade de dígitos ;
 * 
 * @return int - VIOLATE_NUMBER_LENGHT(se o teste deu errado, ou seja, o número não respeita
 *  o limite de dígitos), OKAY ( O número respeita o limite).  
 * */		
int testeTam( int numero, int count){
	//Essa função retorna a quantidade de digitos de um numero
	int Pgroup = 10;
	int Agroup = 10;
	
	int limit = count;
	count = 1;
	
	while( count <= limit){
		
		if( numero < Pgroup){
			if(count == limit)
				return OKAY;
			
		}
		Pgroup *= Agroup;
		count++;
	}
	return VIOLATE_NUMBER_LENGHT;
}

/**
 * Função verifica se um caracter é uma letra.
 * 
 * @param char nome - um caracter a ser comparado;
 * @return int - flag de status
 **/
int verifyCaracter( char nome){
	//Vefirica se o caracter é uma letra
	if((nome >= 'a' && nome <= 'z') || (nome >='A' && nome <= 'Z')) // de A até z na tabela ascii
		return 1;
	
	return 0;

}

/**
 * Função que faz algumas validações na hora de criar os metadados.
 * Funcionamento : Primeiramente verifica se o nome possui apenas letras,
 * depois analisa se o tipo é um formato válido.
 * 
 * @param field estrutura[] - vetor de atributos a serem analisados;
 * @param int numberAtt - Quantidade de atributos no vetor;
 * 
 * @return int - VALOR_INVALIDO ( Não possui apenas letra no nome, ou o tipo
 * é inválido) OKAY ( Valores corretos);
 * */
int checkMeta( field estrutura[], int numberAtt){
	//Checa algumas situações
	int i = 0;
	int j = 0;
	
	
	while( j < numberAtt){ 
		while( estrutura->fName[i] != '\0'){
			if( !verifyCaracter(estrutura->fName[i])) // de A até z na tabela ascii
				return VALOR_INVALIDO;
			
				i++;
		}
		i=0;
		if(!(estrutura->fType == 'S' || estrutura->fType == 'I') || (estrutura->fType == 'D' || estrutura->fType == 'C'))	
			return VALOR_INVALIDO;
		j++;
	}
	return OKAY;
}

int createAttibutes( int id, field *atributos, int numberAtt){
	//Essa função cria os elementos da tabela criada, conforme passada por parâmetro
	
	FILE *metadados = fopen("files/fs_coluna.dat", "a+");  // a+ ( abre o arquivo para R-W, caso não existir, ele será criado.)
	if(!metadados)
		return FILE_META_NOT_FOUND;
	
	int i = 0;
	int getErro;
	int valor;
	
	fseek(metadados,0,SEEK_END); 
	long int pos = ftell(metadados);//tamanho do arquivo
	rewind(metadados);
	
	while( i < pos){ //Procura se já existe  o id criado no arquivo
		
		fread(&valor,sizeof(int),1,metadados);
		if( valor == id){
			return VALUE_ALREADY_EXISTS;
		}
		i += CONST_META;
		fseek( metadados, CONST_META - sizeof(int), SEEK_CUR);
	}
	i = 0;
	
	while( i < numberAtt){ //caso não existe, será gravado no arquivo os valores passados pelo parâmetro
		if((getErro = checkMeta(atributos, numberAtt) == OKAY)){ //Checa alguns formatos válidos nos atributos
			fwrite( &id, sizeof(int), 1 , metadados);
			
			fwrite( atributos[i].fName, sizeof(char), TNAME_LENGHT, metadados);
			fwrite( &atributos[i].fType, sizeof(char), 1, metadados);
			fwrite( &atributos[i].fLenght, sizeof(int),1, metadados);
			i++;
		}
			
		else{ fclose(metadados);  return getErro;}
	}
	fclose(metadados);
	return OKAY;
}

/**
 * O objetivo dessa função é  gerar um nome física para o arquivo de dados.
 * De modo que fique sequêncial ex: file_1.dat,file_2.dat,file_3.dat;
 * Nesta configuração, cada arquivo pertence a uma tabela
 * Funcionamento: Primeiramente extrai o número da string. Que neste caso,
 * sempre estará após um 'underscore' e antes de um ponto. Este número então,
 * é passado para inteiro, e incrementado. Após isso, é colocado em uma nova
 * string de maneira que esteja em sequência com o anterior. 
 * 
 * @param char *last - string com o nome físico da última tabela gravado em disco,
 * se não houver algum então, a string estará definida como : 'file_0.dat'
 * 
 * @return char * - Devolve a string com o nome físico gerado. 
 **/
 char *GeneratePhysName( char *last){
	
	char caracter = 'a';
	bool isNumber = false;
	char *number;
	int shift = 0;
	int i = 0;
	number = (char *) (malloc(sizeof(char)*2));
	
	if(!number){
		return NULL;
	}
	int max = strlen(last);
	
	while( i < max){ //Este while procura o número do arquivo dentro da string.
		caracter = last[i]; //Assuma que o número sempre estará após o underscore e antes do .dat
		if(caracter == '_'){
			isNumber = true;
		}
		else if(isNumber){
			number[shift] = caracter;
			shift++;
			if(last[i+1] == '.'){
				number[shift] = '\0';
				i = max;
			}
		}
		i++;
	}
	long int num = strtol(number, NULL, BASE); //Conversao da string apontada por (number) em inteiro
	
	num++;
	
	char *fnome;
	
	fnome = ( char *) malloc(sizeof(char)* CONST);
	if(!fnome)
		return NULL;
	
    
    char const digitos[] = "0123456789";
	char* pointer = fnome;
    //Este trecho pega um número inteiro e faz a representação dele em forma de string
    //Apenas usada para geração do nome do arquivo de dados
		shift = num;
    do{ //Acha o final da representação do número em string
        ++pointer;
        shift = shift/10;
    }while(shift);
    *pointer = '\0';
    do{ //Cálculo para pegar a representação da variável num
        *--pointer = digitos[num%10];
        num = num/10;
     
    }while(num);
    
    char *string;
    string = (char *)malloc(sizeof(char)*20);
    strcpy(string, "file_");
	strcat( string, fnome);
	strcat( string, ".dat");
	
	free(fnome);
	free(number);
	
	return string;
	
}

int createTable( char *TableName, field *Attributes, int numberAtt){
	
	if(Attributes == NULL || numberAtt <= 0)
		return ABORT;
	
	FILE *arquivo = fopen("files/fs_tabela.dat", "a+"); //Localização fixa
	
	if(!arquivo){
		return FILE_NOT_FOUND;}
	
	fseek(arquivo,0,SEEK_END); 
	long total = ftell(arquivo); //Tamanho do arquivo
		
	rewind(arquivo); //coloca o ponteiro na posição zero
	int i = 0;
	
	criar *ConstTab = malloc(sizeof(criar));
	if(!ConstTab){
		return 0;
	}
	ConstTab->id = 0;
	strcpy(ConstTab->fnome,"file_0\0");
	strcpy(ConstTab->dir, "files/data/\0"); //Valores default
	
	for(;i < total;){ //Este laço percorre o arquivo e procura se já existe a tabela
		fread( &ConstTab->id, sizeof(int), 1, arquivo);	
		fread( ConstTab->lnome, sizeof(char), CONST, arquivo);
		fread( ConstTab->fnome, sizeof(char), CONST, arquivo); 
		fread( ConstTab->dir, sizeof(char), CONST, arquivo);
		
		if( strcmp( ConstTab->lnome, TableName) == 0) {free(ConstTab);return TABLE_ALREADY_EXISTS;} //caso a tabela já exista
		
		i += CONST_MAX ; 
	}
	
	ConstTab->id ++;
	i = 0;
	while( i < strlen(TableName)){
		if(!verifyCaracter(TableName[i]))
			return VALOR_INVALIDO;
		i++;
	}
	strcpy(ConstTab->lnome, TableName);


	char *getFromReturn = GeneratePhysName(ConstTab->fnome);
	if(!getFromReturn)
		return OUT_MEMORIA;
		
	strcpy( ConstTab->fnome, getFromReturn);
	int getErro = createAttibutes( ConstTab->id, Attributes, numberAtt);
	
	if(getErro == OKAY){
		fwrite(&ConstTab->id, sizeof(int), 1, arquivo);
		fwrite(ConstTab->lnome, sizeof(char), CONST, arquivo);
		fwrite(ConstTab->fnome, sizeof(char), CONST, arquivo);
		fwrite(ConstTab->dir, sizeof(char), CONST, arquivo);
	}
	
	fclose(arquivo);
	free(ConstTab);
	free(getFromReturn);
	
	return getErro;
}
int insertInto( char *tableName, Element_t *Attributes){
	
	if(!Attributes || !tableName || !Attributes)
		return ABORT;
	
		
	FILE *table = fopen("files/fs_tabela.dat", "r");
	if(!table)
		return FILE_NOT_FOUND;
	
	FILE *metadados = NULL;
	FILE *newFile = NULL;;
	
	int copiar = 0;
	long int pos = -1;
	int i = 0;
	
	char *diretorio = NULL;
	criar *myTable = NULL;
	myTable = (criar *)malloc(sizeof(criar));
	
	
	int getErro = searchTable(table, tableName, &myTable);
	
	
	if(getErro != OKAY)
		return getErro;
	
	
	
	metadados = fopen("files/fs_coluna.dat", "r");
	if(!metadados)
		return FILE_META_NOT_FOUND;
		
	fseek(metadados,0,SEEK_END);
	long total = ftell(metadados); //Tamanho do arquivo
	rewind(metadados);
		
	
	field *mDados;
	int AttCount = 0;
	while(i < total){ //Procura a posicao inicial do metadados
	
		fread( &copiar, sizeof(int), 1, metadados);
		
		if(copiar == myTable->id){
			pos = ftell(metadados) - sizeof(int);
			break;
		}
			
		else{
			fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
			i += CONST_META;
		}
	}
	if( pos == -1)
		return TABLE_NOT_FOUND; //Table Not Found
		
	fseek(metadados, pos, SEEK_SET);
	i = pos;
	while(i < total){ //Conta os atributos 
		fread( &copiar, sizeof(int), 1, metadados);
		if(myTable->id != copiar)
			break;
		fseek( metadados, CONST_META - sizeof(int) , SEEK_CUR);
		AttCount ++;
		i = ftell(metadados);
	}
	mDados = (field *)malloc(sizeof(field) * AttCount);
	
	fseek(metadados, pos, SEEK_SET);
	
	copiar = 0;
	int endLoop = 0;
	int globalID;
	while( (endLoop < total) && (copiar < AttCount)){
		fread( &globalID, sizeof(int), 1, metadados);
		fread( mDados[copiar].fName, sizeof(char), TNAME_LENGHT, metadados);
		fread( &mDados[copiar].fType, sizeof(char),1, metadados);
		fread( &mDados[copiar].fLenght, sizeof(int), 1, metadados);
		
		copiar++;
		
		endLoop += CONST_META;
	}
	
	/*if(Quantidade != AttCount) //caso o número de atributos no parâmetro for menor ou maior que a quantidade de att.
		return ERRO_ATT_NUMBER;
	*/	
	copiar = 0;
	
	while( copiar < AttCount){
		if(mDados[copiar].fType == 'I' && Attributes[copiar].type == Nint){
			
			if(testeTam(*Attributes[copiar].Dint, mDados[copiar].fLenght) != OKAY){
				free(mDados);
				free(diretorio);
				free(myTable);
				fclose(metadados);
				
				return VIOLATE_NUMBER_LENGHT;
			}
		}
		copiar++;
	}
	
	diretorio = (char *)malloc(sizeof(char)* CONST);
	
	strcpy(diretorio, myTable->dir);
	strcat(diretorio, myTable->fnome);
	newFile = fopen(diretorio, "a+");
	
	if(!newFile){
		if(!(newFile=fopen(diretorio,"w+"))){
			return FILE_DATA_NOT_FOUND;
		}
	}
	copiar = 0; 
	 
	while( copiar < AttCount){
		
		if(mDados[copiar].fType == 'S' && Attributes[copiar].type == String){
			fwrite( Attributes[copiar].Str,sizeof(char), mDados[copiar].fLenght, newFile);
		}
		else if( mDados[copiar].fType == 'C' && Attributes[copiar].type == Caracter){ 
			fwrite(Attributes[copiar].Str, sizeof(char),1, newFile);
		}	
		else if(mDados[copiar].fType == 'D' && Attributes[copiar].type == Ndouble){
			fwrite( Attributes[copiar].Ddouble, sizeof(double), 1, newFile);
		}
		else{
			fwrite( Attributes[copiar].Dint, sizeof(int),1, newFile);
		}
		copiar++;
	}
	free(mDados);
	free(diretorio);
	free(myTable);
	fclose(metadados);
	fclose(newFile);
	return OKAY;
}
