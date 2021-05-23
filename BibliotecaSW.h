#ifndef BIBLIOTECASW_H
#define BIBLIOTECASW_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct noFuncionario{
    char nome[50];
    int id;
    int idade;
    struct noFuncionario *prox;
}NoFuncionario;

typedef struct noCarro{
    char placa;
    int valor; // valor multiplicado pelo tempo. (12 reais/hora)
    struct noCarro *prox;
}NoCarro;

NoCarro* empilharCarroRua(NoCarro *topo, int id, int custo);
NoCarro* empilharCarro(NoCarro *topo, int id, int custo, int totalCarros, FILE *arquivo);
NoCarro* empilharCarro2(NoCarro *topo, int id, int custo);
NoCarro* desempilharCarro(NoCarro **topo);
NoFuncionario* removerDaFila(NoFuncionario **fila);
void cadastrarFuncionarios2(NoFuncionario **filaFunc,char *nome, int id, int idade);
void imprimirPilha(NoCarro *topo);
void cadastrarFuncionarios(NoFuncionario **filaFunc, char *nome, int id, int idade, FILE *arquivo);
void imprimirFila(NoFuncionario *filaFunc);
int gerarHora();
int gerarValores();
void ordenarFuncionariosNome(NoFuncionario *filaFunc);
void ordenarFuncionariosIdade(NoFuncionario *filaFunc);
void ordenarFuncionariosID(NoFuncionario *filaFunc);

#endif

void cadastrarFuncionarios2(NoFuncionario **filaFunc,char *nome, int id, int idade)
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
    }else{
        printf("Memoria indisponivel\n");
    }
}

void ordenarFuncionariosNome(NoFuncionario *filaFunc) 
{
    NoFuncionario *pi; //referencia do primeiro da fila
    NoFuncionario *pj; //referencia do nó que varrerá a fila
    NoFuncionario *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaFunc; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFunc; pj->prox != pfim; pj = pj->prox){
            if ( strcmp(pj->nome, pj->prox->nome) > 0 ){
                int auxNome[50];
                strcpy(auxNome, pj->nome);
                int auxId = pj->id;
                int auxIdade = pj->idade;

                strcpy(pj->nome, pj->prox->nome);
                pj->id = pj->prox->id;
                pj->idade = pj->prox->idade;

                strcpy(pj->prox->nome, auxNome);
                pj->prox->id = auxId;
                pj->prox->idade = auxIdade; 
            }
        }
        pfim = pj;
    }
}

void ordenarFuncionariosIdade(NoFuncionario *filaFunc) 
{
    NoFuncionario *pi; //referencia do primeiro da fila
    NoFuncionario *pj; //referencia do nó que varrerá a fila
    NoFuncionario *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaFunc; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFunc; pj->prox != pfim; pj = pj->prox){
            if (pj->idade > pj->prox->idade){
                int auxNome[50];
                strcpy(auxNome, pj->nome);
                int auxId = pj->id;
                int auxIdade = pj->idade;

                strcpy(pj->nome, pj->prox->nome);
                pj->id = pj->prox->id;
                pj->idade = pj->prox->idade;

                strcpy(pj->prox->nome, auxNome);
                pj->prox->id = auxId;
                pj->prox->idade = auxIdade; 
            }
        }
        pfim = pj;
    }
}

void ordenarFuncionariosID(NoFuncionario *filaFunc) 
{
    NoFuncionario *pi; //referencia do primeiro da fila
    NoFuncionario *pj; //referencia do nó que varrerá a fila
    NoFuncionario *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaFunc; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFunc; pj->prox != pfim; pj = pj->prox){
            if (pj->id > pj->prox->id){
                int auxNome[50];
                strcpy(auxNome, pj->nome);
                int auxId = pj->id;
                int auxIdade = pj->idade;

                strcpy(pj->nome, pj->prox->nome);
                pj->id = pj->prox->id;
                pj->idade = pj->prox->idade;

                strcpy(pj->prox->nome, auxNome);
                pj->prox->id = auxId;
                pj->prox->idade = auxIdade; 
            }
        }
        pfim = pj;
    }
}




NoCarro* empilharCarroRua(NoCarro *topo, int id, int custo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    return novo;
}

NoCarro* empilharCarro(NoCarro *topo, int id, int custo, int totalCarros, FILE *arquivo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    fprintf(arquivo, "Carro %d entrou. Total = %d\n",id,totalCarros);
    return novo;
}

NoCarro* empilharCarro2(NoCarro *topo, int id, int custo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    return novo;
}

int checarValorEstadia(NoCarro *topo, int placa)
{
    while(topo){
        if (topo->placa == placa){
            return topo->valor;
        }else{
            topo = topo->prox;
        }
    }
}

NoFuncionario* removerDaFila(NoFuncionario **fila)
{
    NoFuncionario *remover = NULL;
    remover = *fila;
    *fila = remover->prox;

    return remover;
}

NoCarro* desempilharCarro(NoCarro **topo)
{
    NoCarro *remover = *topo;
    *topo = remover->prox;

    return remover;   
}

void imprimirPilha(NoCarro *topo)
{
    printf("\n-------- PILHA CARROS --------\n");     
    while(topo){   //enquanto topo for true, ou seja, enquanto nao for false/null
        printf("Carro %d\t Custo da estadia: %d\n",topo->placa, topo->valor);
        topo = topo->prox; //interação
    }
    printf("-------- FIM PILHA --------\n\n");
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
        printf("\n----------FILA FUNCIONARIOS---------\n");
        while(filaFunc){
            printf("Funcionario %d: Nome: %s\tIdade: %d\n", filaFunc->id, filaFunc->nome ,filaFunc->idade);
            filaFunc = filaFunc->prox;
        }
    }
}

int checarQtdCarros(NoCarro *topo)
{
    int cont=0;
    while(topo){
        cont++;
        topo = topo->prox;
    }
    return cont;
}

int gerarHora()
{
    int aux;
    do
    {
        aux = rand() % 721; //rand gera um numero entre 0 e 721-1
    } while (aux < 500 );  //forçar a gerar valores altos de horas
    return aux;
}

int gerarValores()
{
    int hora = gerarHora();
    int custo;
    
    if(hora <= 60) custo = 12; 
    else if(hora > 60 && hora <= 120) custo = 12 * 2;
    else if(hora > 120 && hora <= 180) custo = 12 * 3;
    else if(hora > 180 && hora <= 240) custo = 12 * 4;
    else if(hora > 240 && hora <= 300) custo = 12 * 5;
    else if(hora > 300 && hora <= 360) custo = 12 * 6;
    else if(hora > 360 && hora <= 420) custo = 12 * 7;
    else if(hora > 420 && hora <= 480) custo = 12 * 8;
    else if(hora > 480 && hora <= 540) custo = 12 * 9;
    else if(hora > 540 && hora <= 600) custo = 12 * 10;
    else if(hora > 600 && hora <= 660) custo = 12 * 11;
    else if(hora > 660 && hora <= 720) custo = 12 * 12;
  
    return custo;
}