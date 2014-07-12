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

Alterado por EDSLUC:
função void bufferInsert para 
int bufferInsert
