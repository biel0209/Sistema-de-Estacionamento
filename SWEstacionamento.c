#include <stdio.h>
#include <stdlib.h>
#include "BibliotecaSW.h"
#define qtdFuncionarios 3

//TODO: NUMERO DE FUNCIONARIOS: 3         NUMERO TOTAL DE CARROS: 9

int main()
{
    NoCarro *topo = NULL;
    NoFuncionario *filaFunc = NULL;

    char nome[50]; int idade;
    for (int i=0; i<qtdFuncionarios; i++){
        printf("Digite o nome do funcionario: ");
        scanf("%s",nome);
        printf("Digite a idade do funcionario: ");
        scanf("%d",&idade);
        cadastrarFuncionarios(&filaFunc, nome,(i+1),idade);
    }

    imprimirFila(filaFunc);

    return 0;
}
