#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define BUFFER 50

#define TIPO_NAO_NUMERO 0
#define TIPO_INTEIRO_POSITIVO 1
#define TIPO_INTEIRO_NEGATIVO 2
#define TIPO_REAL 3
#define DECIMAL 10

// Grupo: Petrus - Engenharia de Software, Lucas Santana - Ciência da computação, Thiago Chehab - Ciência da computação 
struct lista_no
{
    char info[BUFFER];
    ListaNo *proximo;
    ListaNo *anterior;
    int elements_atual;
    char linhas[100];
};

struct lista
{
    ListaNo *primeiro;
    ListaNo *ultimo;
    int info;
    int elements;

};

Lista *lst_cria(void)
{
    Lista *l = (Lista*)malloc(sizeof(Lista));
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->elements = 0;
    return l;
}

Lista *lst_insere(Lista *l, char *buffer)
{
    ListaNo *novo = (ListaNo *)malloc(sizeof(ListaNo));
    if (novo == NULL)
    {
        return NULL;
    }
    strcpy(novo->info, buffer);

    if (l->primeiro == NULL)
    {
        l->primeiro = novo;
        l->ultimo = novo;
    }
    else
    {
        l->ultimo->proximo = novo;
        novo->anterior = l->ultimo;
        novo->proximo = NULL;
        l->ultimo = novo;
    }
    l->elements++;
    novo->elements_atual = l->elements;
    return l;
}

void remove_enter(char *str)
{
    int length = strlen(str);
    if (str[length - 1] == '\n')
        str[length - 1] = '\0';
}

int carrega_dados(FILE *arq, Lista *comandos)
{
    char buffer[BUFFER];
    int erro = 0;
    for (int i = 0; !feof(arq); i++)
    {
        fgets(buffer, BUFFER, arq);
        remove_enter(buffer);
        remove_comentario(buffer);
        erro = remove_linha_vazia(buffer);
        if(erro == -1)
            continue;
            
        //remove_leading_white_space(buffer);
        //remove_trailing_white_space(buffer);
        if (lst_insere(comandos, buffer) == NULL)
        {
            return -1;
        }
    }
}

int remove_linha_vazia(char *str)
{

    for (int i=0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ')
        {
            continue;
        }
        if (str[i] == '\n')
            return -1;

        if(isdigit(str[i]))
            return 0;
    }
    return -1;
}


void lst_libera(Lista *l)
{
    ListaNo *p = l->primeiro;
    while (p != NULL)
    {
        ListaNo *t  = p->proximo; 
        free(p);
        p = t; 
    }

    free(l);
}

int verifica_tipo(char *str)
{
    char *end_ptr;
    int val_h = strtol(str, &end_ptr, DECIMAL);
    if (*end_ptr == '\0')
    {
        if (val_h >= 0)
            return TIPO_INTEIRO_POSITIVO;

        return TIPO_INTEIRO_NEGATIVO;
    }

    float val_f = strtod(str, &end_ptr);
    if (*end_ptr == '\0')
        return TIPO_REAL;

    return TIPO_NAO_NUMERO;
}

void procura_erro(Lista *componentes, int linha, FILE * log, int total_linhas)
{
    
    char *comandos1;
    int conta = 0;
    int inicio = 0, aux;
    char read[] = "read";
    char add[] = "add";
    char store[] = "store";
    char storeconst[] = "storeconst";
    char sub[] = "sub";
    char jump[] = "jump";
    char write[] = "write";
    char div[] = "div";
    char mult[] = "mult";

    char *comando = componentes->primeiro->info;
    if (strcmp(comando, read) == 0
        || strcmp(comando, write) == 0
        || strcmp(comando, store) == 0)
        verifica_read_write_store(componentes, log, linha);
    
    else if (strcmp(comando, storeconst) == 0)
        verifica_storeconst(componentes, log, linha);
    
    else if (strcmp(comando, add) == 0
             || strcmp(comando, sub) == 0
             || strcmp(comando, mult) == 0
             || strcmp(comando, div) == 0)
        op_aritmetica(componentes, log, linha);
    
    else if (strcmp(comando, jump) == 0)
        verifica_jump(componentes, log, linha, total_linhas);

    else
    {
        fprintf(log, "\nLinha %d: Comando incorreto!\n", linha);
        printf("Linha %d: Comando incorreto!\n", linha);
    }
}

void verifica_read_write_store(Lista *componentes, FILE *log, int linha)
{
    char *comando = componentes->primeiro->info;
    if (componentes->elements != 2)
    {
        fprintf(log, "Linha %d: Numero de parametros do comando %s esta incorreto!\n", linha, comando);
        return;
    }
    int arg1 = verifica_tipo(componentes->primeiro->proximo->info);
    if (arg1 != TIPO_INTEIRO_POSITIVO)
    {
        fprintf(log, "Linha %d: Necessario apenas numeros naturais para o comando %s!\n", linha, comando);
        printf("\nLinha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    }
}


void verifica_storeconst(Lista *componentes, FILE *log, int linha)
{
    char *comando = componentes->primeiro->info;
   
    if (componentes->elements != 3)
    {
        fprintf(log, "Linha %d: Numero de parametros do comando %s esta incorreto!\n", linha, comando);
        return;
    }
    int arg1 = verifica_tipo(componentes->primeiro->proximo->info);
    if(arg1 == TIPO_NAO_NUMERO)
        fprintf(log, "\nLinha %d: Necessario apenas numeros nos valores colocados. O que foi digitado: '%s'", linha, comando);
        printf("Linha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    
    int arg2 = verifica_tipo(componentes->primeiro->proximo->proximo->info);
    if (arg2 != TIPO_INTEIRO_POSITIVO)
    {
        fprintf(log, "Esperado um segundo numero natural para o parametro storeconst. Passado '%s'", linha, comando);
        printf("Linha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    }
}

void op_aritmetica(Lista *componentes, FILE *log, int linha)
{
    char *comando = componentes->primeiro->info;
    if (componentes->elements != 3)
    {
        fprintf(log, "Linha %d: Numero de parametros do comando %s esta incorreto!\n", linha, comando);
        return;
    }
    int arg1 = verifica_tipo(componentes->primeiro->proximo->info);
    if (arg1 != TIPO_INTEIRO_POSITIVO)
    {
        fprintf(log, "Linha %d: Necessario apenas numeros naturais para o comando %s!\n", linha, comando);
        printf("Linha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    }
    int arg2 = verifica_tipo(componentes->primeiro->proximo->proximo->info);
    if (arg2 != TIPO_INTEIRO_POSITIVO)
    {
        fprintf(log, "Linha %d: Necessario apenas numeros naturais para o comando %s!\n", linha, comando);
        printf("Linha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    }

}


void verifica_jump(Lista *componentes, FILE *log, int linha, int total_linhas)
{
    char *comando = componentes->primeiro->info;
    if (componentes->elements != 3)
    {
        fprintf(log, "Linha %d: numero de parametros do comando %s, eh incorreto!\n", linha, comando);
        return;
    }
    
    int tipo_arg1 = verifica_tipo(componentes->primeiro->proximo->info);
    if (tipo_arg1 != TIPO_INTEIRO_POSITIVO)
    {
        fprintf(log, "Linha %d: Necessario apenas numeros naturais nos parametros!\n", linha);
        printf("Linha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    }

    int tipo_arg2 = verifica_tipo(componentes->primeiro->proximo->proximo->info);
    if (tipo_arg2 != TIPO_INTEIRO_POSITIVO && tipo_arg2 != TIPO_INTEIRO_NEGATIVO)
    {
        fprintf(log, "Linha %d: O delta do jump faz a execução ir para fora das instrucoes do codigo.\n", linha);
        printf("Linha: %d: Necessario apenas numeros naturais nos parametros!\n", linha);
    }
    else
    {
        
        int offset = strtol(componentes->primeiro->proximo->proximo->info, NULL, DECIMAL);
        int linha_final = linha + offset;
        if(linha_final < 0 || linha_final > total_linhas)
        {
            fprintf(log, "Linha %d:O offset do jump desloca o fluxo de execucao para fora do codigo\n", linha);
        }
    }  
        
}

void remove_comentario(char *comandos)
{
    char *end_ptr = NULL;
    end_ptr = strstr(comandos, "#");
    if(end_ptr != NULL)
        *end_ptr = '\0';

}

void gera_lista_componentes(char *comando, Lista *l)
{
    int inicio, fim;
    char buffer[BUFFER];

    for (int i = 0; strlen(comando)>i; i++)
    {
        
        while(comando[i] == ' ')
            i++;  
        
        if(comando[i] == '\0')
        {
            break;
        } 
        inicio = i;

        while(comando[i] != ' ' && comando[i] != '\0')
            i++;
  
        fim = i;

        strncpy(buffer, comando + inicio, fim - inicio);
        buffer[fim-inicio] = '\0';
        if(lst_insere(l, buffer) == NULL)
        {
            printf("Erros nos comandos inseridos");
        }

    }
}

int analisa_comandos(Lista *l, FILE * log)
{

    ListaNo *p = l->primeiro;
    int linhas = 1;
    int total_linhas = l->elements;

    while (p != NULL)
    {
        Lista* componentes = lst_cria();
        gera_lista_componentes(p->info, componentes);
        procura_erro(componentes, linhas, log, total_linhas);
        linhas++;
        p = p->proximo;
        printf("\n");
        lst_libera(componentes);
    
    }
    
}