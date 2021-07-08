#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TAM_ESTAC 6
#define QTD_TESTES 3
#define M 11     // tamanho da tabela hash = (2*TAM_ESTAC) e numero primo mais proximo

typedef struct arv{
    int placa;
    int id;
    struct arv *dir;
    struct arv *esq;  
}Tree;

typedef struct avl{
    int placa;
    int altura; //distancia de um nó x até seu filho mais afastado
    struct avl *dir;
    struct avl *esq;  
}AvlTree;

typedef struct registroCarros{ // Tipo responsável por gravar as ações para replicar os mesmos eventos
    int placa;
    char atividade[15]; // Pode ser -> "entrou" ou "devera sair".
    struct registroCarros *prox;
}RegistroCarros;

typedef struct noHash{ // lista para tabela hash
    int placa;
    int id;
    struct noHash *prox;
}noHash;

typedef struct listaHash{ // lista para tabela hash
    noHash *inicio;
}ListaHash;





RegistroCarros *carrosSimulacao = NULL; //instancia da simulação
int idxREV;
int qtdCarros = 0; //qtd de carros na arvore

int numArvIN = 0;  //numero de pesquisas de entrada na arvore de busca binaria
int numArvOU = 0;  //numero de pesquisas de saida na arvore de busca binaria
float mediaArvIN = 0;  //numero medio de pesquisas de entrada na arvore de busca binaria
float mediaArvOU = 0;  //numero medio de pesquisas de saida na arvore de busca binaria

int numHashIN = 0;  //numero de pesquisas de entrada na tabela hash
int numHashOU = 0;  //numero de pesquisas de saida na tabela hash
float mediaHashIN = 0;  //numero medio de pesquisas de entrada na tabela hash
float mediaHashOU = 0;  //numero medio de pesquisas de saida na tabela hash

int checarDuplicados = 0;


int maior(int x, int y);
int fatorB(AvlTree *avl);  //calcular fator de balanceamento da arvore avl;
void rotacaoDE(AvlTree **avl);  //rotacao dupla a direita
void rotacaoED(AvlTree **avl);  //rotacao dupla a esquerda
void rotacaoSD(AvlTree **avl); //rotacao simples a direita
void rotacaoSE(AvlTree **avl);  //rotacao simples a esquerda
int inserirAvl(AvlTree **avl, int placa);
int alturaNo(AvlTree *avl);
void inserirTabela(ListaHash hash[], int placa, int id);
void inserirLista(ListaHash *lista, int placa, int id);
void inicializarLista(ListaHash *lista);
void inicializarTabela(ListaHash hash[]);
int espalhamento(int chave);


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

void inserirArv(Tree **arv, int placa, int id)
{
   
    if (*arv == NULL){
        *arv = malloc(sizeof(Tree));
        (*arv)->placa = placa;
        (*arv)->id = id;
        (*arv)->esq = NULL;
        (*arv)->dir = NULL;
    }
    else{
        if (placa < (*arv)->placa){
            numArvIN++;
            inserirArv(&((*arv)->esq),placa,id);
        }else if(placa > (*arv)->placa){
            numArvIN += 2;
            inserirArv(&((*arv)->dir),placa,id);
        }else{
            printf("dup");
            checarDuplicados = 1;
        }
    }
}

Tree* removerArv(Tree *arv, int placa)
{
    if (arv==NULL){
        return NULL;
    }else{
        if(arv->placa == placa){ 
            numArvOU++;
            if (arv->esq == NULL && arv->dir == NULL){  //condição para removerArv uma folha
                idxREV = arv->id;
                free(arv);
                return NULL;
            }else if (arv->esq == NULL || arv->dir == NULL){ //condição para removerArv raiz com 1 filho
                Tree *aux;
                if (arv->esq != NULL){
                    aux = arv->esq;
                }else{
                    aux = arv->dir;
                }
                idxREV = arv->id;
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
                idxREV = arv->id;
                return arv;
            }
        }else{
            if (placa < arv->placa){
                numArvOU += 2;
                arv->esq = removerArv(arv->esq, placa);
            }else{
                numArvOU += 2;
                arv->dir = removerArv(arv->dir, placa);
            }
            return arv;
        }
    }
}

void alterarIndiceARV(Tree **arv)
{
   if (*arv != NULL){
        alterarIndiceARV(&((*arv)->esq));
        if((*arv)->id > idxREV){
            (*arv)->id = ((*arv)->id) - 1;
        }
        alterarIndiceARV(&((*arv)->dir));
    }
}

void verificarPortao(int placa)
{
    int portao;
    if (qtdCarros%2 == 0) {
        if (qtdCarros/2 >= idxREV) portao = 1;
        else portao = 2;
    }else {
        int aux = (qtdCarros/2) + 1;
        if (aux > idxREV) portao = 1;
        else portao = 2;
    }
    printf("\nO carro %d saiu pelo portao %d\n", placa, portao);
}

int carroPraRemover(Tree *arv, int cont, int random)
{   
    while (arv != NULL && cont < random) {
        if ((arv)->esq) 
            (arv) = (arv)->esq;
        else if((arv)->dir)
            (arv) = (arv)->dir;
        else
            return (arv)->placa;
        cont++;
    }
    return (arv)->placa;
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

void imprimirERD(Tree **arv)
{
    if (*arv != NULL){
        imprimirERD(&((*arv)->esq));
        printf("%d ",(*arv)->placa);
        imprimirERD(&((*arv)->dir));
    }
}

int inserirAvl(AvlTree **avl, int placa)
{
    int res; 

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
    aux->altura = maior(alturaNo(aux->esq), (*avl)->altura) + 1;
    *avl = aux;
}

void rotacaoSD(AvlTree **avl)  //rotacao simples a direita
{
    AvlTree *aux;
    aux = (*avl)->dir;
    (*avl)->dir = aux->esq;
    aux->esq = *avl;
    (*avl)->altura = maior(alturaNo((*avl)->esq), alturaNo((*avl)->dir)) + 1;
    aux->altura = maior(alturaNo(aux->dir), (*avl)->altura) + 1;
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

void inserirTabela(ListaHash hash[], int placa, int id)
{
    int indice = espalhamento(placa);
    inserirLista(&hash[indice],placa,id);
}

void inserirLista(ListaHash *lista, int placa, int id)
{   
    noHash *aux = malloc(sizeof(noHash));
    aux->placa = placa;
    aux->id = id;
    aux->prox = lista->inicio;
    lista->inicio = aux;
}

void inicializarLista(ListaHash *lista)
{
    lista->inicio = NULL;
}

void inicializarTabela(ListaHash hash[])
{
    for(int i=0; i<M; i++)
        inicializarLista(&hash[i]); 
}

int espalhamento(int chave)
{
    return chave % M;
}

void alterarIndiceHASH2(ListaHash *lista)
{
    noHash *aux;
    aux = lista->inicio;
    while(aux){
        if(aux->id > idxREV)
            aux->id -= 1;
        aux = aux->prox;
    } 
}

void alterarIndiceHASH(ListaHash hash[])
{
    for(int i=0; i<M; i++)
        alterarIndiceHASH2(&hash[i]); 
}

void removerLista(ListaHash *lista, int placa)
{
    noHash *aux, *remover = NULL;
    if(lista){
        if(lista->inicio->placa == placa){
            numHashOU++;
            remover = lista->inicio;
            lista->inicio = remover->prox;
            idxREV = remover->id;
        }else{
            numHashOU++;;
            aux = lista->inicio;
            while(aux->prox && aux->prox->placa != placa){
                numArvOU++;
                aux = aux->prox; 
            }
            numHashOU++;
            if(aux->prox){
                remover = aux->prox;
                aux->prox = remover->prox;
                idxREV = remover->id;
            }
        }
    }
}

void removerHash(ListaHash hash[], int chave)
{
    int indice = espalhamento(chave);
    removerLista(&hash[indice],chave);
}

void imprimirListaHash(ListaHash *lista)
{
    noHash *aux;
    aux = lista->inicio;
    while(aux){
        printf("%d ",aux->placa);
        aux = aux->prox;
    } 
}

void imprimirHash(ListaHash hash[])
{   
    for(int i=0; i<M; i++)
        imprimirListaHash(&hash[i]); 
}

int main()
{
    srand(time(NULL));
    int placa;
    Tree *arv = NULL;
    AvlTree *avl = NULL;
    ListaHash hash[M];
    inicializarTabela(hash);

    // INSERCAO NA ARVORE DE BUSCA BINARIA
    printf("||||||||||||||||||||||||||||||||||||||\tARVORE DE BUSCA BINARIA\t||||||||||||||||||||||||||||||||||||||:\n");
    for(int j=0; j<QTD_TESTES; j++){
        for (int i=0; i<TAM_ESTAC; i++){
            placa = rand() % 100; //gerar placas de carros de 0 a 100
            inserirArv(&arv, placa,i+1);
            while (checarDuplicados == 1){
                checarDuplicados = 0;
                placa = rand() % 100; //gerar placas de carros de 0 a 100
                inserirArv(&arv, placa,i+1);
            }
            qtdCarros++;
            insereRegistro(&carrosSimulacao, placa, "entrou");
        } 

        printf("\n----------Impressao %d da Arvore de busca Binaria----------\n",j+1);
        imprimirERD(&arv);

        while(arv != NULL){
            //printf("\nremover qual placa?\n");
            //scanf("%d",&placa);
            placa = carroPraRemover(arv,0,rand()%qtdCarros);
            arv = removerArv(arv,placa);
            verificarPortao(placa);
            qtdCarros--;
            alterarIndiceARV(&arv);
            insereRegistro(&carrosSimulacao, placa, "saiu");
            imprimirERD(&arv);
        }
        numArvIN += numArvIN;
        numArvOU += numArvOU;
    }
    mediaArvIN = numArvIN/QTD_TESTES;
    mediaArvOU = numArvOU/QTD_TESTES;

    printf("\nNumero medio de pesquisas de entrada da arvore de busca binaria: %.2f\n",mediaArvIN);
    printf("\nNumero medio de pesquisas de saida da arvore de busca binaria: %.2f\n",mediaArvOU);
    

    imprimirLista(carrosSimulacao);

    //INSERCAO NA TABELA HASH
    printf("||||||||||||||||||||||||||||||||||||||\tTABELA HASH\t||||||||||||||||||||||||||||||||||||||:\n");
    for(int j=0; j<QTD_TESTES; j++){
        qtdCarros=0;
        RegistroCarros *registro;
        registro = carrosSimulacao;
        for(int i=0; i<TAM_ESTAC; i++){
            placa = registro->placa;
            inserirTabela(hash,placa,i+1);
            qtdCarros++;
            registro = registro->prox;
        }
        printf("\n------------Impressao %d da tabela hash------------\n",j+1);
        imprimirHash(hash);

        for(int i=0; i<TAM_ESTAC; i++){
            placa = registro->placa;
            removerHash(hash,placa);
            verificarPortao(placa);
            qtdCarros--;
            alterarIndiceHASH(hash);
            imprimirHash(hash);
            registro = registro->prox;
        }
        numHashIN += numHashIN;
        numHashOU += numHashOU;
    }   
    mediaHashIN = numHashIN/QTD_TESTES;
    mediaHashOU = numHashOU/QTD_TESTES;
    printf("\nNumero medio de pesquisas de entrada da tabela hash: %.2f\n",mediaHashIN);
    printf("\nNumero medio de pesquisas de saida da tabela hash: %.2f\n",mediaHashOU);





    /* INSERCAO NA ARVORE AVL
   for (int i=0; i<TAM_ESTAC; i++){
        scanf("%d",&placa);
        int x = inserirAvl(&avl, placa);
        if (x==0) printf("%d nao inserido!\n",placa);
        insereRegistro(&carrosSimulacao, placa, "entrou");
    } */

    //imprimirAvl(&avl);

    //imprimirLista(carrosSimulacao);
    return 0;
}