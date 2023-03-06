
#ifndef HEADER
#define HEADER

typedef struct lista_no ListaNo;
typedef struct lista Lista;
// Grupo: Petrus - Engenharia de Software, Lucas Santana - Ciência da computação, Thiago Chehab - Ciência da computação

Lista *lst_cria(void);
int carrega_dados(FILE *arq, Lista *comandos);
Lista *lst_insere(Lista *l, char *buffer);
int verifica_tipo(char *str);
void comandoss(char *comandos, ListaNo *h, int *linhas);
int analisa_comandos(Lista *l, FILE*log);
void verifica_read_write_store(Lista * componentes, FILE *log, int linha);
void op_aritmetica(Lista *componentes, FILE *log, int linha);
void verifica_add(Lista *componentes, FILE *log, int linha);
void verifica_storeconst(Lista *componentes, FILE *log, int linha);
void verifica_add(Lista *componentes, FILE *log, int linha);
void verifica_jump(Lista *componentes, FILE *log, int linha, int total_linhas);
void verifica_div(Lista *componentes, FILE *log, int linha);
void verifica_mult(Lista *componentes, FILE *log, int linha);
void procura_erro(Lista * componentes, int linha, FILE * log, int total_linhas);
void remove_comentario(char *comando);
void remove_enter(char *str);
int remove_linha_vazia(char *str);
void gera_lista_componentes(char *comando, Lista *l);
void lst_libera(Lista *l);



#endif