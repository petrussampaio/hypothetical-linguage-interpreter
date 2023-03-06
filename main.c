#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define BUFFER 50
/// Grupo: Petrus - Engenharia de Software, Lucas Santana - Ciência da computação, Tiago Chehab - Ciência da computação
int main(int argc, char const *argv[])
{
    char *path;
    int cont = 0;
    if (argc != 3)
    {
        printf("Eh necessario passar o path do arquivo como argumento!");
        return -1;
    }

    FILE *f = fopen(argv[1], "r");
    FILE *log = fopen(argv[2], "w");
    if(log == NULL || f == NULL)
    {
        printf("Arquivo nao foi aberto");
        return -1;
    }


    int *linhas_comandos;

    Lista *comandos = lst_cria();
    carrega_dados(f, comandos);
    analisa_comandos(comandos, log);
    lst_libera(comandos);
    

    return 0;
}
