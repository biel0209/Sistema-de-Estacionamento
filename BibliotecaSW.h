#ifndef BIBLIOTECASW_H
#define BIBLIOTECASW_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct noFuncionario{
    char nome[50];
    int id;
    int idade;
    struct noFuncionario *prox;
}NoFuncionario;

typedef struct noCarro{
    char id;
    char placa[6];
    struct noCarro *prox;
}NoCarro;
//


void cadastrarFuncionarios(NoFuncionario **filaFunc, 
    char *nome, int id, int idade, FILE *arquivo);
void imprimirFila(NoFuncionario *filaFunc);


#endif

void empilharCarro(NoCarro *topo, char id, char *placa)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->id = id;
    strcpy(novo->placa, placa);
    novo->prox = topo;
}

void imprimirPilha(NoCarro *topo)
{
    printf("\n-------- PILHA CARROS --------\n");     
    while(topo){   //enquanto topo for true, ou seja, enquanto nao for false/null
        printf("ID: %c\tPlaca: %s\n",topo->id, topo->placa);
        topo = topo->prox; //interação
    }
    printf("\n-------- FIM PILHA --------\n");
}

void cadastrarFuncionarios(NoFuncionario **filaFunc, 
    char *nome, int id, int idade, FILE *arquivo)
{
    NoFuncionario *aux, *novo = malloc(sizeof(NoFuncionario));
    if (novo){
        strcpy(novo->nome, nome);
        novo->id = id;
        novo->idade = idade;
        novo->prox = NULL;
        if (*filaFunc == NULL){
            *filaFunc = novo;
        }else{
            aux = *filaFunc;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
        //gravando no arquivo
        fprintf(arquivo, "Funcionario de nome %s (ID = %d; Idade = %d) cadastrado\n", novo->nome, novo->id, novo->idade);
    }else{
        printf("Memoria indisponivel\n");
    }
}

void imprimirFila(NoFuncionario *filaFunc)
{
    if (filaFunc == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("----------FILA FUNCIONARIOS---------\n");
        while(filaFunc){
            printf("Funcionario %d: Nome: %s\tIdade: %d\n", filaFunc->id, filaFunc->nome ,filaFunc->idade);
            filaFunc = filaFunc->prox;
        }
    }
}


