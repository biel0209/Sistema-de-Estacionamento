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
    char placa;   //TODO: placa deve ser inteiro
    int valor; // valor multiplicado pelo tempo. (12 reais/hora)
    struct noCarro *prox;
}NoCarro;

typedef struct regCarro{
    char placa;
    int valor; // valor multiplicado pelo tempo. (12 reais/hora)
    char acao[15]; //acao que foi realizada com um determinado carro ("entrou", "devera sair")
    struct regCarro *prox;
}RegCarro;

typedef struct listaCarro{
    char placa;
    int valor; // valor multiplicado pelo tempo. (12 reais/hora)
    struct listaCarro *prox;
    struct listaCarro *ant;
}ListaCarro;

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
int checarQtdCarrosLista(ListaCarro *lista);

#endif

void insereNaListaReg(RegCarro **lista, char placa, int custo, char *acao) //insere no fim
{
    RegCarro *novo = malloc(sizeof(RegCarro));
    RegCarro *aux;
    
    novo->placa = placa;
    novo->valor = custo;
    strcpy(novo->acao, acao);
    novo->prox = NULL;

    if(*lista == NULL){
        *lista = novo;
    }else{
        aux = *lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}


void insereNaLista(ListaCarro **lista, ListaCarro **listaFim, char placa, int custo, int totalCarros, FILE *arquivoDia2) //insere no fim
{
    ListaCarro *novo = malloc(sizeof(ListaCarro));
    ListaCarro *aux;
    
    novo->placa = placa;
    novo->valor = custo;
    novo->prox = NULL;
    novo->ant = *listaFim;

    if(*lista == NULL){
        *lista = novo;
        *listaFim = novo; 
    }else{
        aux = *lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
        *listaFim = aux->prox;
    }
    fprintf(arquivoDia2, "Carro %d entrou. Total = %d\n", novo->placa, totalCarros);
}

void insereNaListaFim(ListaCarro **lista, ListaCarro **listaCarrosFim, char placa, int custo) //insere no fim
{
    ListaCarro *novo = malloc(sizeof(ListaCarro));
    
    novo->placa = placa;
    novo->valor = custo;
    novo->prox = NULL;
    novo->ant = *listaCarrosFim;
    if(*listaCarrosFim == NULL){
        *listaCarrosFim = novo;
        *lista = novo;
    }
    else{
        *listaCarrosFim = novo;
    }
}

void insereNaListaCabeca(ListaCarro **lista, ListaCarro **listaCarrosFim, char placa, int custo) //insere no fim
{
    ListaCarro *novo = malloc(sizeof(ListaCarro));

    novo->placa = placa;
    novo->valor = custo;
    novo->prox = *lista;
    novo->ant = NULL;

    if((*lista) == NULL){
        *lista = novo;
        *listaCarrosFim = novo;
    }else{
        *lista = novo;
    }

    
}

int checarTamInicio(ListaCarro **lista, int placa) //verificar quantos carros tem entre o inicio da lista e o carro procurado
{
    ListaCarro *aux;
    int cont=0;
    aux = *lista;
    while(aux && aux->placa != placa){
        cont++;
        aux = aux->prox;
    }
    return cont;
}

int checarTamFim(ListaCarro **listaFim, int placa) //verificar quantos carros tem entre o inicio da lista e o carro procurado
{
    ListaCarro *aux;
    int cont=0;
    aux = *listaFim;
    while(aux && aux->placa != placa){
        cont++;
        aux = aux->ant;
    }
    return cont;
}

ListaCarro* removerDaLista(ListaCarro **lista, ListaCarro **listaCarrosFim)
{   
    ListaCarro *remover = NULL;
    if(*lista){
        if ((*lista)->prox == (*listaCarrosFim)){
            remover = *lista;
            *lista = remover->prox;
            (*listaCarrosFim)->ant = NULL;  
        }
        else{
            if((*lista) == (*listaCarrosFim)){     //TODO: verificar esse else if, debugar pois ta bugando
                remover = *lista;
                *lista = remover->prox;
                *listaCarrosFim = NULL;
            }else{
                remover = *lista;
                *lista = remover->prox;
            }
            
        }
        
    }
    return remover;
}

ListaCarro* removerDaListaFim(ListaCarro **lista, ListaCarro **listaCarrosFim)
{   
    ListaCarro *remover = NULL;  
    if(*listaCarrosFim){
        if((*listaCarrosFim)->ant == (*lista)){
            remover = *listaCarrosFim;
            *listaCarrosFim = remover->ant;
            (*lista)->prox = NULL;
        }else{
            if((*listaCarrosFim) == (*lista)){
                remover = *listaCarrosFim;
                *listaCarrosFim = remover->ant;
                *lista = NULL;
            }else{
                remover = *listaCarrosFim;
                *listaCarrosFim = remover->ant;
            }
        }
        
        
    }
    return remover;
}


void imprimirListaReg(RegCarro *lista)
{
    printf("\n------------------------- REGISTRO DO FUNCIONAMENTO DO ESTACIONAMENTO -------------------------\n");
    while(lista){
        printf("Carro %d\tAcao: %s\tEstadia: %d\n",lista->placa,lista->acao,lista->valor);
        lista = lista->prox;
    }
}

void imprimirLista(ListaCarro *lista)
{
    printf("\n-------------------------LISTA DE CARROS -------------------------\n");
    while(lista){
        printf("Carro %d\tEstadia: %d\n",lista->placa,lista->valor);
        lista = lista->prox;
    }
}

void imprimirListaFim(ListaCarro *lista)
{
    printf("\n-------------------------LISTA DE CARROS INVERSA -------------------------\n");
    while(lista){
        printf("Carro %d\tEstadia: %d\n",lista->placa,lista->valor);
        lista = lista->ant;
    }
}

int checarQtdCarrosLista(ListaCarro *lista)
{
    int cont=0;
    while(lista){
        cont++;
        lista = lista->prox;
    }
    return cont;
}

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

void ordenarFuncionariosNome(NoFuncionario *filaFunc)       //metodo bubblesort
{
    NoFuncionario *pi; //referencia do primeiro da fila
    NoFuncionario *pj; //referencia do nó que varrerá a fila
    NoFuncionario *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaFunc; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFunc; pj->prox != pfim; pj = pj->prox){
            if ( strcmp(pj->nome, pj->prox->nome) > 0 ){
                char auxNome[50];
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

void ordenarFuncionariosIdade(NoFuncionario *filaFunc)    //metodo bubblesort
{
    NoFuncionario *pi; //referencia do primeiro da fila
    NoFuncionario *pj; //referencia do nó que varrerá a fila
    NoFuncionario *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaFunc; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFunc; pj->prox != pfim; pj = pj->prox){
            if (pj->idade > pj->prox->idade){
                char auxNome[50];
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

void ordenarFuncionariosID(NoFuncionario *filaFunc)     //metodo bubblesort
{
    NoFuncionario *pi; //referencia do primeiro da fila
    NoFuncionario *pj; //referencia do nó que varrerá a fila
    NoFuncionario *pfim = NULL; //referencia do ultimo da fila

    for(pi=filaFunc; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFunc; pj->prox != pfim; pj = pj->prox){
            if (pj->id > pj->prox->id){
                char auxNome[50];
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