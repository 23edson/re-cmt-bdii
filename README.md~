Feito até agora por RAPEDU e após por CACMATTHI


FUNÇÕES:

void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount);

void findNextAvaliable(buffer *bPool);

void applyReplacementPolicies(buffer *bPool);

void bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);

int fillBuffer(buffer *bufferPool, field *fieldList,char *nomeTabela, char *arquivo);

int showBuffer(buffer *bufferPool);


TABELAS:

fs_personagem em fs_personagem.dat

fs_status em fs_status.dat

Dicionario de Dados:

dicionario.dat

Estrutura do dicionario.dat
<nome tabela>'\0'<nome tabela>.dat'\0'

Estruturas das tabelas
int(numero de colunas) praticamente igual do primeiro '.dat'
nome campo
tipo campo
tamanho campo

~~~~~~~~~~~~~~~~~~~~~~~~~~Alterado por EDSLUC:
funções: 

int bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght);

int fillBuffer(buffer **bufferPool, char *nomeTabela, int contador);

int getTupleNumber(FILE *arquivo, int position, int tamTuple);

Element_t *extractTupleFromBP(buffer *bufferPool ,int tupleNumber);

int createTable( char *TableName, field *Attributes, int numberAtt);

int insertInto( char *tableName, Element_t *Attributes,bool overWrite,int positionTuple);

int counter( int init, FILE *metadados, int total, criar *myTable );

Dicionario:
files/fs_tabela.dat
estrutura:
id; 	identificador 1 inteiro
lnome;  nome tabela: TNAME_LENGHT ou seja 20 characteres
fnome;  nome fisico: TNAME_LENGHT characteres
dir;    diretório: TNAME_LENGHT characteres

para achar arquivo de dados: dir concatenado com fnome;
padrão: files/data/<nomearquivo>.dat

Colunas da tabela:
files/fs_coluna.dat
estrutura:

id; da tabela
(nome coluna
tipo
tamanho)*numero de colunas
para cada tabela


