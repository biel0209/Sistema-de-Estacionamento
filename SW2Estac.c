#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define QTD_CARROS 5

typedef struct arv{
    int placa;
    struct arv *dir;
    struct arv *esq;  
}Tree;

typedef struct avl{
    int placa;
    int altura;
    struct avl *dir;
    struct avl *esq;  
}AvlTree;

typedef struct registroCarros{ // Tipo responsável por gravar as ações para replicar os mesmos eventos
    int placa;
    char atividade[15]; // Pode ser -> "entrou" ou "devera sair".
    struct registroCarros *prox;
}RegistroCarros;

RegistroCarros *carrosSimulacao = NULL; //instancia da simulação


int maior(int x, int y);
int fatorB(AvlTree *avl);  //calcular fator de balanceamento da arvore avl;
void rotacaoDE(AvlTree **avl);  //rotacao dupla a direita
void rotacaoED(AvlTree **avl);  //rotacao dupla a esquerda
void rotacaoSD(AvlTree **avl); //rotacao simples a direita
void rotacaoSE(AvlTree **avl);  //rotacao simples a esquerda
int inserirAvl(AvlTree **avl, int placa);
int alturaNo(AvlTree *avl);


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

void inserirArv(Tree **arv, int placa)
{
    if (*arv == NULL){
        *arv = malloc(sizeof(Tree));
        (*arv)->placa = placa;
        (*arv)->esq = NULL;
        (*arv)->dir = NULL;
    }
    else{
        if (placa < (*arv)->placa){
            inserirArv(&((*arv)->esq),placa);
        }else if(placa > (*arv)->placa){
            inserirArv(&((*arv)->dir),placa);
        }else{
            printf("dup");
        }
    }
}

Tree* removerArv(Tree *arv, int placa)
{
    if (arv==NULL){
        return NULL;
    }else{
        if(arv->placa == placa){ 
            if (arv->esq == NULL && arv->dir == NULL){  //condição para removerArv uma folha
                free(arv);
                return NULL;
            }else if (arv->esq == NULL || arv->dir == NULL){ //condição para removerArv raiz com 1 filho
                Tree *aux;
                if (arv->esq != NULL){
                    aux = arv->esq;
                }else{
                    aux = arv->dir;
                }
                free(arv);
                return aux;
            }else{                                               //condicao para removerArv raiz com 2 filhos
                Tree *aux = arv->esq;
                while (aux->dir != NULL){
                    aux = aux->dir;
                }
                arv->placa = aux->placa;
                aux->placa = placa;
                arv->esq = removerArv(arv->esq, placa);
                return arv;
            }
        }else{
            if (placa < arv->placa){
                arv->esq = removerArv(arv->esq, placa);
            }else{
                arv->dir = removerArv(arv->dir, placa);
            }
            return arv;
        }
    }
}

int getPaiTree(Tree **arv, int placa) //retornar o pai de um argumento placa
{
    if (*arv == NULL){
        return -1;
    }else{
        if( (*arv)->esq && (*arv)->esq->placa == placa || (*arv)->dir && (*arv)->dir->placa == placa){
            return (*arv)->placa;
        }else{
            if(placa > (*arv)->placa){
                return getPaiTree(&((*arv)->dir), placa);
            }else if(placa < (*arv)->placa){
                return getPaiTree(&((*arv)->esq), placa);
            }
        }
    }
}

void imprimirRED(Tree **arv)
{
    if (*arv != NULL){
        printf("%d ",(*arv)->placa);
        imprimirRED(&((*arv)->esq));
        imprimirRED(&((*arv)->dir));
    }
}

void imprimirERD(Tree **arv)
{
    if (*arv != NULL){
        imprimirERD(&((*arv)->esq));
        printf("%d ",(*arv)->placa);
        imprimirERD(&((*arv)->dir));
    }
}

void imprimirEDR(Tree **arv)
{
    if (*arv != NULL){
        imprimirEDR(&((*arv)->esq));
        imprimirEDR(&((*arv)->dir));
        printf("%d ",(*arv)->placa);
    }
}

int inserirAvl(AvlTree **avl, int placa)
{
    int res; //testar depois sem ele

    if (*avl == NULL){ //arvore vazia ou estão ja chegou na folha
        AvlTree *aux;
        aux = (AvlTree*) malloc(sizeof(AvlTree));
        if (aux == NULL) return 0;  //se aux falhar em alocar memoria a funcao retorna 0
        aux->placa = placa;
        aux->altura = 0;
        aux->esq = NULL;
        aux->dir = NULL;
        *avl = aux;
        return 1;   //inserido com sucesso
    }

    AvlTree *atual = *avl;
    if (placa < atual->placa){ //se a placa for menor que a placa de atual, insere pela esquerda
        if((res = inserirAvl(&(atual->esq), placa)) == 1){ //se o retorno for true, sucesso na insercao
            if(fatorB(atual) >= 2){  //se o fator em modulo for maior igual 2 precisa balancear
                if(placa < (*avl)->esq->placa) 
                    rotacaoSE(avl);
                else 
                    rotacaoED(avl);
            }
        }
    }else{   //se a placa for maiorr que a placa de atual, insere pela direita
        if(placa > atual->placa){
            if((res = inserirAvl(&(atual->dir), placa)) == 1){ //se o retorno for true, sucesso na insercao
                if(fatorB(atual) >= 2){  //se o fator em modulo for maior igual 2 precisa balancear
                    if(placa > (*avl)->dir->placa) 
                        rotacaoSD(avl);
                    else 
                        rotacaoDE(avl);
                }
            }
        }else{    //se a placa nao é maior nem menor, ela é igual, valor duplicado
            printf("\nValor duplicado!\n");
            return 0;
        }
    }
    atual->altura= maior(alturaNo(atual->esq), alturaNo(atual->dir)) + 1;

    return res;
}

void rotacaoSE(AvlTree **avl)  //rotacao simples a esquerda
{
    AvlTree *aux;
    aux = (*avl)->esq;
    (*avl)->esq = aux->dir;
    aux->dir = *avl;
    (*avl)->altura = maior(alturaNo((*avl)->esq), alturaNo((*avl)->dir)) + 1;
    aux->altura = maior(alturaNo((*avl)->esq), alturaNo((*avl)->dir)) + 1;
    *avl = aux;
}

void rotacaoSD(AvlTree **avl)  //rotacao simples a direita
{
    AvlTree *aux;
    aux = (*avl)->dir;
    (*avl)->dir = aux->esq;
    aux->esq = *avl;
    (*avl)->altura = maior(alturaNo((*avl)->esq), alturaNo((*avl)->dir)) + 1;
    aux->altura = maior(alturaNo((*avl)->dir), alturaNo(*avl)) + 1;
    *avl = aux;
}

void rotacaoED(AvlTree **avl)  //rotacao dupla a esquerda
{
    rotacaoSD(&(*avl)->esq);
    rotacaoSE(avl);
}

void rotacaoDE(AvlTree **avl)  //rotacao dupla a direita
{
    rotacaoSE(&(*avl)->dir);
    rotacaoSD(avl);
}

int alturaNo(AvlTree *avl)  //calcular fator de balanceamento da arvore avl
{
    if(avl == NULL) return -1;
    else return avl->altura;
}

int fatorB(AvlTree *avl)  //calcular fator de balanceamento da arvore avl
{
    return labs(alturaNo(avl->esq) - alturaNo(avl->dir));
}

int maior(int x, int y)
{
    return (x > y ? x:y);
}

void imprimirAvl(AvlTree **avl)
{
    if (*avl != NULL){
        imprimirAvl(&((*avl)->esq));
        printf("Placa: %d Altura: %d\n",(*avl)->placa, (*avl)->altura);
        imprimirAvl(&((*avl)->dir));
    }
}

int main()
{
    Tree *arv = NULL;
    AvlTree *avl = NULL;
    int placa;
    /*
    for (int i=0; i<QTD_CARROS; i++){
        scanf("%d",&placa);
        inserirArv(&arv, placa);
        insereRegistro(&carrosSimulacao, placa, "entrou");
    }
    */

   for (int i=0; i<QTD_CARROS; i++){
        scanf("%d",&placa);
        int x = inserirAvl(&avl, placa);
        if (x==0) printf("%d nao inserido!\n",placa);
        insereRegistro(&carrosSimulacao, placa, "entrou");
    }

    imprimirAvl(&avl);

    /*
    printf("\nImpressao ERD:\n");
    imprimirERD(&arv);
    */

    while(arv != NULL){
        printf("\nremover qual placa?\n");
        scanf("%d",&placa);
        arv = removerArv(arv,placa);
        insereRegistro(&carrosSimulacao, placa, "saiu");
        imprimirERD(&arv);
    }

    imprimirLista(carrosSimulacao);
    
    return 0;
}