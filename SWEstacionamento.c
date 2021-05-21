#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define qtdFuncionarios 2
#define tamEstacionamento 4

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

NoCarro* empilharCarroRua(NoCarro *topo, char id, int custo);
NoCarro* empilharCarro(NoCarro *topo, char id, int custo, int totalCarros, FILE *arquivo);
NoCarro* empilharCarro2(NoCarro *topo, char id, int custo);
NoCarro* desempilharCarro(NoCarro **topo);
void imprimirPilha(NoCarro *topo);
void cadastrarFuncionarios(NoFuncionario **filaFunc, 
    char *nome, int id, int idade, FILE *arquivo);
void imprimirFila(NoFuncionario *filaFunc);
int gerarHora();
int gerarValores();

int main()
{
    int montanteEstacionamento = 0; // montante do arrecadado no estacionamento.
    //Criando ponteiro para arquivo
    FILE *arquivo;      // criando a variável ponteiro para o arquivo
    arquivo = fopen("log.txt", "a");   //abrindo o arquivo
    
    if (arquivo == NULL){    //Testando a abertura do arquivo
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }
    //Fim da criacao do ponteiro para arquivo


    NoCarro *rua = NULL;
    NoCarro *topo = NULL;  //estrutura do estacionamento (pilha)
    NoFuncionario *filaFunc = NULL; //estrutura dos funcionarios (fila)

    //Procedimento pra receber do usuario informacoes sobre o funcionario e cadastra-los
    char nome[50]; int idade;
    for (int i=0; i<qtdFuncionarios; i++){
        printf("Digite o nome do funcionario: ");
        scanf("%s",nome);
        printf("Digite a idade do funcionario: ");
        scanf("%d",&idade);
        cadastrarFuncionarios(&filaFunc, nome,(i+1),idade,arquivo);
    }
    //Fim do cadastramento

    //Colocar carros no estacionamento
    
    fprintf(arquivo, "Ordenacao usada: ID;\n");
    fprintf(arquivo, "Abertura do estacionamento (lotacao maxima = %d).\n", tamEstacionamento);

    if (topo == NULL)
        fprintf(arquivo, "Estacionamento vazio!\n");

    //Colocando carros na pilha
    srand(time(NULL));
    for (int i=0; i<tamEstacionamento; i++){
        char id = (char)((int)'A' + i); 
        int custo = gerarValores();
        topo = empilharCarro(topo,id,custo,i+1,arquivo);
    }

    char resposta;
    printf("Deseja retirar qual carro?\n");
    scanf("%s", resposta);

    NoCarro* tempAux;

    int cont = 0;

    for (int i = 0; i < tamEstacionamento; i++) {
        
        // desempilha os carros
        tempAux = desempilharCarro(&topo);
 
        // empilha os carros na rua
        if (tempAux->placa != resposta) {
            rua = empilharCarroRua(rua, tempAux->placa, tempAux->valor);
            cont++;
        } else {
            printf("Carro retirado");
            break;
        }
    }

    for (int i = 0; i < cont; i++) {

        tempAux = desempilharCarro(&rua);

        topo = empilharCarro2(topo, tempAux->placa,
            tempAux->valor);
    }

    imprimirPilha(topo);

    fclose(arquivo); //fechar arquivo

    /*leitura do conteudo de log.txt e impressao no output
    int c;
    FILE *arquivoLeitura;
    arquivoLeitura = fopen("log.txt", "r");
    while ((c = getc(arquivoLeitura)) != EOF)
        putchar(c);
    fclose(arquivoLeitura);
    */

    //imprimirFila(filaFunc); //imprimir fila de funcionarios
    
    return 0;
}

NoCarro* empilharCarroRua(NoCarro *topo, char id, int custo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    return novo;
}

NoCarro* empilharCarro(NoCarro *topo, char id, int custo, int totalCarros, FILE *arquivo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    fprintf(arquivo, "Carro %c entrou. Total = %d\n",id,totalCarros);
    return novo;
}

NoCarro* empilharCarro2(NoCarro *topo, char id, int custo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    return novo;
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
        printf("Carro %c\t Custo da estadia: %d\n",topo->placa, topo->valor);
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

int gerarHora()
{
    int aux;

    do
    {
        aux = rand() % 361;
    } while (aux == 0);

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
  
    return custo;
}
