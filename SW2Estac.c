#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define QTD_CARROS 5

typedef struct tree{
    int placa;
    struct tree *dir;
    struct tree *esq;  
}Arvore;

typedef struct registroCarros{ // Tipo responsável por gravar as ações para o segundo dia.
    int placa;
    char atividade[15]; // Pode ser -> "entrou" ou "devera sair".
    struct registroCarros *prox;
}RegistroCarros;

RegistroCarros *carrosSimulacao = NULL; //instancia da simulação

Arvore* remover(Arvore *arv, int placa)
{
    if (arv==NULL){
        return NULL;
    }else{
        if(arv->placa == placa){ 
            if (arv->esq == NULL && arv->dir == NULL){  //condição para remover uma folha
                free(arv);
                return NULL;
            }else if (arv->esq == NULL || arv->dir == NULL){ //condição para remover raiz com 1 filho
                Arvore *aux;
                if (arv->esq != NULL){
                    aux = arv->esq;
                }else{
                    aux = arv->dir;
                }
                free(arv);
                return aux;
            }else{                                               //condicao para remover raiz com 2 filhos
                Arvore *aux = arv->esq;
                while (aux->dir != NULL){
                    aux = aux->dir;
                }
                arv->placa = aux->placa;
                aux->placa = placa;
                arv->esq = remover(arv->esq, placa);
                return arv;
            }
        }else{
            if (placa < arv->placa){
                arv->esq = remover(arv->esq, placa);
            }else{
                arv->dir = remover(arv->dir, placa);
            }
            return arv;
        }
    }
}

void inserir(Arvore **arv, int placa)
{
    if (*arv == NULL){
        *arv = malloc(sizeof(Arvore));
        (*arv)->placa = placa;
        (*arv)->esq = NULL;
        (*arv)->dir = NULL;
    }
    else{
        if (placa < (*arv)->placa){
            inserir(&((*arv)->esq),placa);
        }else if(placa > (*arv)->placa){
            inserir(&((*arv)->dir),placa);
        }else{
            printf("dup");
        }
    }
}

int getPai(Arvore **arv, int placa)
{
    if (*arv == NULL){
        return -1;
    }else{
        if( (*arv)->esq && (*arv)->esq->placa == placa || (*arv)->dir && (*arv)->dir->placa == placa){
            return (*arv)->placa;
        }else{
            if(placa > (*arv)->placa){
                return getPai(&((*arv)->dir), placa);
            }else if(placa < (*arv)->placa){
                return getPai(&((*arv)->esq), placa);
            }
        }
    }
}

void imprimirRED(Arvore **arv)
{
    if (*arv != NULL){
        printf("%d ",(*arv)->placa);
        imprimirRED(&((*arv)->esq));
        imprimirRED(&((*arv)->dir));
    }
}

void imprimirERD(Arvore **arv)
{
    if (*arv != NULL){
        imprimirERD(&((*arv)->esq));
        printf("%d ",(*arv)->placa);
        imprimirERD(&((*arv)->dir));
    }
}

void imprimirEDR(Arvore **arv)
{
    if (*arv != NULL){
        imprimirEDR(&((*arv)->esq));
        imprimirEDR(&((*arv)->dir));
        printf("%d ",(*arv)->placa);
    }
}

void insereRegistro(RegistroCarros **carrosSimulacao, int placa, char *atividade)
{
    RegistroCarros *novoCarro = malloc(sizeof(RegistroCarros));
    RegistroCarros *auxiliar;

    novoCarro->placa = placa;
    strcpy(novoCarro->atividade, atividade);
    novoCarro->prox = NULL;

    if(*carrosSimulacao == NULL){
        *carrosSimulacao = novoCarro;
    }else{
        auxiliar = *carrosSimulacao;
        while (auxiliar->prox != NULL)
        {
            auxiliar = auxiliar->prox;
        }
        auxiliar->prox = novoCarro;
    }
}

void imprimirLista(RegistroCarros *carrosSimulacao)
{
    if(carrosSimulacao == NULL){
        printf("A lista esta vazia!\n");
    }else{
        while(carrosSimulacao){
            printf("Placa: %d // Atividade: %s\n", carrosSimulacao->placa, carrosSimulacao->atividade);
            carrosSimulacao = carrosSimulacao->prox;
        }
    }
}


int main()
{
    Arvore *arv = NULL;
    int placa;
    for (int i=0; i<QTD_CARROS; i++){
        scanf("%d",&placa);
        inserir(&arv, placa);
        insereRegistro(&carrosSimulacao, placa, "entrou");
    }

    printf("\nImpressao ERD:\n");
    imprimirERD(&arv);

    while(arv != NULL){
        printf("\nRemover qual placa?\n");
        scanf("%d",&placa);
        arv = remover(arv,placa);
        insereRegistro(&carrosSimulacao, placa, "saiu");
        imprimirERD(&arv);
    }

    imprimirLista(carrosSimulacao);
    
    return 0;
}