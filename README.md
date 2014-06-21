Feito até agora por RAPEDU e CACMATTHI


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

Funcionando com o main do grupo anterior:
somente fs_personagem.

Dicionario de Dados:

dicionario.dat


