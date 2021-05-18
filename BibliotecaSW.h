#ifndef BIBLIOTECASW_H
#define BIBLIOTECASW_H

typedef struct noFuncionario{
    char nome[50];
    int id;
    int idade;
    struct noFuncionario *prox;
}NoFuncionario;

typedef struct noCarro{
    char placa[6];
    struct noCarro *prox;
}NoCarro;


void cadastrarFuncionarios(NoFuncionario *headFila, 
    NoFuncionario *tailFila, char *nome, int id, int idade);
void imprimirFila(NoFuncionario *headFila);


#endif


void cadastrarFuncionarios(NoFuncionario *headFila, 
    NoFuncionario *tailFila, char *nome, int id, int idade)
{
    NoFuncionario *novo = malloc(sizeof(NoFuncionario));
    if (novo != NULL){
        strcpy(novo->nome, nome);
        novo->id = id;
        novo->idade = idade;
        novo->prox = NULL;

        if (headFila == NULL){
            headFila = novo;
        }else{
            tailFila->prox = novo;
        }
        tailFila = novo;
    }else{
        printf("Memoria indisponivel\n");
    }
}

void imprimirFila(NoFuncionario *headFila)
{
    if (headFila == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("----------FILA FUNCIONARIOS---------\n");
        while(headFila != NULL){
            printf("Funcionario %d: Nome: %s\tIdade: %d\n", headFila->id, headFila->nome ,headFila->idade);
            headFila = headFila->prox;
        }
    }
}


