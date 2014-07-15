
/*FLAGS DE ERROS. Para o funcionamento correto, as flags são todas números negativos. 
 * 
 **/
#define OKAY -1 
/* OKAY - define um status de sucesso, este valor é retornado caso a função obteve êxito;*/

#define FILE_NOT_FOUND -2
/* FILE_NOT_FOUND - Quando o arquivo fs_tabela.dat não é localizado;*/

#define VALUE_ALREADY_EXISTS -3
/* VALUE_ALREADY_EXISTS - Retorna se o item que identifica a tabela já existe; */

#define VALOR_INVALIDO -4
/* VALOR_INVALIDO - Controla algumas situações onde existe um limite para os valores analisados. ex: Nome pode ser apenas letra; */

#define ERRO_ATT_NUMBER -5
/* ERRO_ATT_NUMBER - O número de atributos de uma tabela é diferente do que a tabela realmente possui; */

#define VIOLATE_NUMBER_LENGHT -6
/* VIOLATE_NUMBER_LENGHT - Exclusivo para atributos que são números inteiro, onde o limite de dígitos interessa; */

#define TABLE_ALREADY_EXISTS -7
/* TABLE_ALREADY_EXISTE - A tabela em questão já existe em disco; */

#define TABLE_NOT_FOUND -8
/* TABLE_NOT_FOUND - Retorna se a tabela pesquisada NÃO existe no arquivo; */

#define FILE_META_NOT_FOUND -9
/* FILE_META_NOT_FOUND - Arquivo fs_coluna.dat não foi localizado no disco ;*/

#define FILE_DATA_NOT_FOUND -10
/* FILE_DATA_NOT_FOUND - Arquivo de dados não foi localizado ex: Imagine tabela 1. Seus dados em disco ficão no arquivo file_1.dat; */

#define OUT_MEMORIA -11
/* OUT_MEMORIA - Retorna ao tentar alocar um espaço de memória e não consegue; */

#define TUPLE_NOT_FOUND -12
/* TUPLE_NOT_FOUND - Ao procurar uma tupla, retorna se ela não for encontrada; */

#define EMPTY_BUFFER -13
/* EMPTY_BUFFER - Retorna se o buffer estiver vazio ; */

#define ABORT -14
/* ABORT - Finaliza a função sem retornar um erro específico; */





