#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define QUANTIDADE_FUNCIONARIOS 2 // Quantidade de funcionários.
#define TAMANHO_ESTACIONAMENTO 4 // Capacidade máxima do estacionamento.

typedef struct noFuncionario{ // Tipo responsável por definir um funcionário do estacionamento.
    char nome[50];
    int id; // Identificador único de cada funcionário.
    int idade;
    struct noFuncionario *prox;
}NoFuncionario;

typedef struct noCarro{ // Tipo responsável por definir um carro do estacionamento.
    int placa;
    int valor; // Valor final resultante do produto entre tempo de estadia e custo.
    struct noCarro *prox;
}NoCarro;

typedef struct registroCarros{ // Tipo responsável por gravar as ações para o segundo dia.
    int placa;
    int valor; // Valor final resultante do produto entre tempo de estadia e custo.
    char atividade[15]; // Pode ser -> "entrou" ou "deve sair".
    struct registroCarros *prox;
}RegistroCarros;

typedef struct listaCarros{
    int placa;
    int valor; // // Valor final resultante do produto entre tempo de estadia e custo.
    struct listaCarros *prox; // Posterior.
    struct listaCarros *ant; // Anterior.
}ListaCarros;

RegistroCarros *carrosSimulacao = NULL; // Estrutura secundária para armazenar dados em um registro, usados na simulação posterior.
FILE *arquivo; // Log do primeiro dia.
FILE *arquivoSimulacao; // Log do segundo dia.
ListaCarros *carrosIni = NULL; // Referencia para o inicio da fila.
ListaCarros *carrosFim = NULL; // Referencia para o fim da fila.
NoCarro *topo = NULL; // Estrutura principal do estacionamento de tipo PILHA.
NoCarro *rua = NULL; // Estrutura auxiliar para guardar os carros durante as manobras de retirada.
NoFuncionario *filaFuncionarios = NULL; // Estrutura principal dos funcionários de tipo FILA.
int lucroEstacionamento = 0; // Valor total arrecadado durante o funcionamento do estacionamento.

NoCarro* desempilharCarro(NoCarro **topo);
NoCarro* empilharCarro(NoCarro *topo, int id, int custo, int qtdCarros, FILE *arquivo);
NoCarro* empilharCarroAUX(NoCarro *topo, int id, int custo);
NoCarro* empilharCarroRua(NoCarro *topo, int id, int custo);
NoFuncionario* removerDaFila(NoFuncionario **fila);
void adicionarCarro();
void cadastrarFuncionarios(NoFuncionario **filaFuncionarios, char *nome, int id, int idade, FILE *arquivo);
void cadastrarFuncionariosAUX(NoFuncionario **filaFuncionarios,char *nome, int id, int idade);
void imprimirFila(NoFuncionario *filaFuncionarios);
void imprimirPilha(NoCarro *topo);
void insereListaFim(ListaCarros **carrosIni, ListaCarros **carrosFim, int placa, int valor);
void insereListaIni(ListaCarros **carrosIni, ListaCarros **carrosFim, int placa, int valor);
void insereLista(ListaCarros **carrosIni, ListaCarros **carrosFim,int quantidadeCarros, int placa, int valor, FILE *arquivoSimulacao);
void insereRegistro(RegistroCarros **carrosSimulacao, int resposta, int valor, char *atividade);
void ordenarFuncionariosIdade(NoFuncionario *filaFuncionarios);
void ordenarFuncionariosID(NoFuncionario *filaFuncionarios);
void ordenarFuncionariosNome(NoFuncionario *filaFuncionarios);
void verificarValorEstadia(NoCarro *topo, int placa);
void removerCarro();
void removerListaFim(int placa, int valor);
void removerListaIni(int placa, int valor);
void removerCarrosFechamento();
int contarCarros(ListaCarros *carros);
int gerarHora();
int gerarValores();
int verificarTamanhoFim(ListaCarros **carrosFim, int placa);
int verificarTamanhoIni(ListaCarros **carrosIni, int placa);
int verificarQuantidadeCarrosLista(ListaCarros *carrosIni);
int verificarQuantidadeCarros(NoCarro *topo);

NoCarro* desempilharCarro(NoCarro **topo)
{
    NoCarro *remover = *topo;
    *topo = remover->prox;

    return remover;   
}

NoCarro* empilharCarro(NoCarro *topo, int id, int custo, int qtdCarros, FILE *arquivo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    fprintf(arquivo, "Carro %d entrou. Total = %d\n",id,qtdCarros);
    return novo;
}

NoCarro* empilharCarroAUX(NoCarro *topo, int id, int custo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    return novo;
}

NoCarro* empilharCarroRua(NoCarro *topo, int id, int custo)
{
    NoCarro *novo = malloc(sizeof(NoCarro));
    novo->placa = id;
    novo->prox = topo;
    novo->valor = custo;

    return novo;
}

NoFuncionario* removerDaFila(NoFuncionario **fila)
{
    NoFuncionario *remover = NULL;
    remover = *fila;
    *fila = remover->prox;

    return remover;
}

void adicionarCarro()
{
    int custo = gerarValores();
    int id;

    printf("Digite a placa do carro: ");
    scanf("%d",&id);

    int qtdCarros = verificarQuantidadeCarros(topo);
    topo = empilharCarro(topo,id,custo,qtdCarros+1,arquivo);

    insereRegistro(&carrosSimulacao, id, custo, "entrou");
}

void cadastrarFuncionarios(NoFuncionario **filaFuncionarios, char *nome, int id, int idade, FILE *arquivo)
{
    NoFuncionario *aux, *novo = malloc(sizeof(NoFuncionario));
    if (novo){
        strcpy(novo->nome, nome);
        novo->id = id;
        novo->idade = idade;
        novo->prox = NULL;
        if (*filaFuncionarios == NULL){
            *filaFuncionarios = novo;
        }else{
            aux = *filaFuncionarios;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
        fprintf(arquivo, "Funcionario de nome %s (ID = %d; Idade = %d) cadastrado\n", novo->nome, novo->id, novo->idade);
    }else{
        printf("Memoria indisponivel\n");
    }
}

void cadastrarFuncionariosAUX(NoFuncionario **filaFuncionarios, char *nome, int id, int idade)
{
    NoFuncionario *aux, *novo = malloc(sizeof(NoFuncionario));
    if (novo){
        strcpy(novo->nome, nome);
        novo->id = id;
        novo->idade = idade;
        novo->prox = NULL;
        if (*filaFuncionarios == NULL){
            *filaFuncionarios = novo;
        }else{
            aux = *filaFuncionarios;
            while(aux->prox){
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    }else{
        printf("Memoria indisponivel\n");
    }
}

void imprimirFila(NoFuncionario *filaFuncionarios)
{
    if (filaFuncionarios == NULL){
        printf("A fila esta vazia\n");
    }else{
        printf("\n----------FILA FUNCIONARIOS---------\n");
        while(filaFuncionarios){
            printf("Funcionario %d: Nome: %s\tIdade: %d\n", filaFuncionarios->id, filaFuncionarios->nome ,filaFuncionarios->idade);
            filaFuncionarios = filaFuncionarios->prox;
        }
    }
}

void imprimirPilha(NoCarro *topo)
{
    printf("\n-------- PILHA CARROS --------\n");     
    while(topo){   //enquanto topo for true, ou seja, enquanto nao for false/null
        printf("Carro %d\t Custo da estadia: %d\n",topo->placa, topo->valor);
        topo = topo->prox; //iteração
    }
    printf("-------- FIM PILHA --------\n\n");
}

void insereLista(ListaCarros **carrosIni, ListaCarros **carrosFim,int quantidadeCarros, int placa, int valor, FILE *arquivoSimulacao)
{
    ListaCarros *novoCarro = malloc(sizeof(ListaCarros));
    ListaCarros *aux;

    novoCarro->placa = placa;
    novoCarro->valor = valor;
    novoCarro->prox = NULL;
    novoCarro->ant = *carrosFim;

    if (*carrosIni == NULL)
    {
        *carrosIni = novoCarro;
        *carrosFim  = novoCarro;
    }else{
        aux = *carrosIni;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novoCarro;
        *carrosFim = aux->prox;
    }
    fprintf(arquivoSimulacao, "Carro %d entrou. Total = %d\n", novoCarro->placa, quantidadeCarros);
}

void insereRegistro(RegistroCarros **carrosSimulacao, int resposta, int valor, char *atividade)
{
    RegistroCarros *novoCarro = malloc(sizeof(RegistroCarros));
    RegistroCarros *auxiliar;

    novoCarro->placa = resposta;
    novoCarro->valor = valor;
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

void ordenarFuncionariosIdade(NoFuncionario *filaFuncionarios) 
{
    NoFuncionario *pi; // Início da fila.
    NoFuncionario *pj; // Varredor da fila.
    NoFuncionario *pfim = NULL; // Fim da fila.

    for(pi=filaFuncionarios; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFuncionarios; pj->prox != pfim; pj = pj->prox){
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

void ordenarFuncionariosID(NoFuncionario *filaFuncionarios) 
{
    NoFuncionario *pi; // Início da fila.
    NoFuncionario *pj; // Varredor da fila.
    NoFuncionario *pfim = NULL; // Fim da fila.

    for(pi=filaFuncionarios; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFuncionarios; pj->prox != pfim; pj = pj->prox){
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

void ordenarFuncionariosNome(NoFuncionario *filaFuncionarios) 
{
    NoFuncionario *pi; // Início da fila.
    NoFuncionario *pj; // Varredor da fila.
    NoFuncionario *pfim = NULL; // Fim da fila.

    for(pi=filaFuncionarios; pi->prox != NULL; pi = pi->prox){
        for(pj=filaFuncionarios; pj->prox != pfim; pj = pj->prox){
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

void removerCarro()
{
    // Colocar os carros na rua.
    int resposta;

    printf("Deseja retirar qual carro?: ");
    scanf("%d", &resposta);

    int estadia = checarValorEstadia(topo,resposta);
    fprintf(arquivo, "Carro %d devera sair (Estadia = %d)\n", resposta, estadia);

    insereRegistro(&carrosSimulacao, resposta, estadia, "saira");

    lucroEstacionamento += estadia; // Adiciona ao lucro total o custo do carro.
    
    NoCarro* tempAux;   
    int cont = 0;

    NoFuncionario* funcTrabalhando;
    
    // O laço corre até o carro desejado ser retirado do estacionamento.
    for (int i = 0; i < TAMANHO_ESTACIONAMENTO; i++) {
        funcTrabalhando = removerDaFila(&filaFuncionarios); // Funcionário responsável por retirar o carro.
        tempAux = desempilharCarro(&topo);
        fprintf(arquivo, "-----Carro %d retirado pelo funcionario %s!\n", tempAux->placa,funcTrabalhando->nome);
        if (tempAux->placa != resposta) {  // Enquanto não for o carro desejado, os carros são empilhados na rua.
            rua = empilharCarroRua(rua, tempAux->placa, tempAux->valor);
            cont++;
        }else{
            cadastrarFuncionariosAUX(&filaFuncionarios,funcTrabalhando->nome, funcTrabalhando->id, funcTrabalhando->idade);
            break;
        }
        cadastrarFuncionariosAUX(&filaFuncionarios,funcTrabalhando->nome, funcTrabalhando->id, funcTrabalhando->idade);
    }

    // Laço para retornar os carros de volta ao estacionamento.
    for (int i = 0; i < cont; i++) {
        funcTrabalhando = removerDaFila(&filaFuncionarios); // funcionario que desempilha o carro.
        tempAux = desempilharCarro(&rua);
        topo = empilharCarroAUX(topo, tempAux->placa, tempAux->valor);
        fprintf(arquivo, "-----Carro %d voltou pelo funcionario %s!\n", tempAux->placa,funcTrabalhando->nome);
        cadastrarFuncionariosAUX(&filaFuncionarios,funcTrabalhando->nome, funcTrabalhando->id, funcTrabalhando->idade);
    }

    fprintf(arquivo, "Carro %d saiu! Total = %d\n", resposta, verificarQuantidadeCarros(topo));
}

void removerListaFim(int placa, int valor)
{
    fprintf(arquivoSimulacao, "Carro %d devera sair (Estadia = %d)\n",placa,valor);

    ListaCarros *carroTemp;
    NoCarro *carroRuaTemp;
    NoFuncionario *funcionarioTrabalhando;


    int cont;

    for(int i = 0; i < TAMANHO_ESTACIONAMENTO; i++){
        funcionarioTrabalhando = removerDaFila(&filaFuncionarios);
        carroTemp = removerDaListaFim(&carrosIni, &carrosFim);
        fprintf(arquivoSimulacao, "-----Carro %d retirado pelo funcionario %s!\n", carroTemp->placa,funcionarioTrabalhando->nome);

        if(carroTemp->placa != placa){
            rua = empilharCarroRua(rua, carroTemp->placa, carroTemp->valor);
            cont++;
        }else{
            cadastrarFuncionariosAUX(&filaFuncionarios, funcionarioTrabalhando->nome, funcionarioTrabalhando->id, funcionarioTrabalhando->idade);
            break;
        }
        cadastrarFuncionariosAUX(&filaFuncionarios, funcionarioTrabalhando->nome, funcionarioTrabalhando->id, funcionarioTrabalhando->idade);
    }

    for(int i = 0; i < cont; i++){
        funcionarioTrabalhando = removerDaFila(&filaFuncionarios);
        carroRuaTemp = desempilharCarro(&rua);
        insereListaFim(&carrosIni, &carrosFim, carroRuaTemp->placa, carroRuaTemp->valor);
        fprintf(arquivoSimulacao, "-----Carro %d voltou pelo funcionario %s!\n", carroRuaTemp->placa,funcionarioTrabalhando->nome);
        cadastrarFuncionariosAUX(&filaFuncionarios, funcionarioTrabalhando->nome, funcionarioTrabalhando->id, funcionarioTrabalhando->idade);
    }
    fprintf(arquivoSimulacao, "Carro %d saiu. Total = %d\n", placa, verificarQuantidadeCarrosLista(carrosIni));
}

void removerListaIni(int placa, int valor)
{
    fprintf(arquivoSimulacao, "Carro %d devera sair (Estadia = %d)\n",placa,valor);

    ListaCarros *carroTemp;
    NoCarro *carroRuaTemp;
    NoFuncionario *funcionarioTrabalhando;


    int cont;

    for(int i = 0; i < TAMANHO_ESTACIONAMENTO; i++){
        funcionarioTrabalhando = removerDaFila(&filaFuncionarios);
        carroTemp = removerDaLista(&carrosIni, &carrosFim);
        fprintf(arquivoSimulacao, "-----Carro %d retirado pelo funcionario %s!\n", carroTemp->placa,funcionarioTrabalhando->nome);

        if(carroTemp->placa != placa){
            rua = empilharCarroRua(rua, carroTemp->placa, carroTemp->valor);
            cont++;
        }else{
            cadastrarFuncionariosAUX(&filaFuncionarios, funcionarioTrabalhando->nome, funcionarioTrabalhando->id, funcionarioTrabalhando->idade);
            break;
        }
        cadastrarFuncionariosAUX(&filaFuncionarios, funcionarioTrabalhando->nome, funcionarioTrabalhando->id, funcionarioTrabalhando->idade);
    }

    for(int i = 0; i < cont; i++){
        funcionarioTrabalhando = removerDaFila(&filaFuncionarios);
        carroRuaTemp = desempilharCarro(&rua);
        insereListaIni(&carrosIni, &carrosFim, carroRuaTemp->placa, carroRuaTemp->valor);
        fprintf(arquivoSimulacao, "-----Carro %d voltou pelo funcionario %s!\n", carroRuaTemp->placa,funcionarioTrabalhando->nome);
        cadastrarFuncionariosAUX(&filaFuncionarios, funcionarioTrabalhando->nome, funcionarioTrabalhando->id, funcionarioTrabalhando->idade);
    }
    fprintf(arquivoSimulacao, "Carro %d saiu. Total = %d\n", placa, verificarQuantidadeCarrosLista(carrosIni));
}

void removerCarrosFechamento()
{
    NoFuncionario* funcTrabalhando; // Funcionario responsavel pela retirada do carro.
    NoCarro* tempAux;
    int quantidadeAux = 0;
    
    while (topo)
    {
        funcTrabalhando = removerDaFila(&filaFuncionarios); // Funcionário responsável por retirar o carro.
        tempAux = desempilharCarro(&topo); // Retirada dos carros que estavam no estaionamento.
        quantidadeAux = verificarQuantidadeCarros(topo); // Após a retirada dos carros, é verificado a quantidade.

        fprintf(arquivo, "Carro %d devera sair (estadia = %d reais.)\n",tempAux->placa,tempAux->valor);
        fprintf(arquivo, "-----Carro %d retirado pelo funcionario %s!\n", tempAux->placa,funcTrabalhando->nome);
        fprintf(arquivo, "Carro %d saiu! Total = %d", tempAux->placa, quantidadeAux);

        cadastrarFuncionariosAUX(&filaFuncionarios,funcTrabalhando->nome, funcTrabalhando->id, funcTrabalhando->idade);

    }
}

int contarCarros(ListaCarros *carrosIni)
{
    int aux = 0;

    while (carrosIni != NULL)
    {
        aux++;
        carrosIni = carrosIni->prox;
    }
    
}

int gerarHora()
{
    int aux;
    do
    {
        aux = rand() % 721; // A função rand gera números 0-720 (0-(n-1)).
    } while (aux < 500 );  // Pra facilitar, são gerados valores altos.
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

int verificarTamanhoFim(ListaCarros **carrosFim, int placa)
{
    ListaCarros *aux;
    int cont;

    aux = *carrosFim;

    while (aux && aux->placa != placa)
    {
        cont++;
        aux = aux->ant;
    }
    return cont;
}

int verificarTamanhoIni(ListaCarros **carrosIni, int placa)
{
    ListaCarros *aux;
    int cont;

    aux = *carrosIni;

    while (aux && aux->placa != placa)
    {
        cont++;
        aux = aux->prox;
    }
    return cont;
}

int verificarQuantidadeCarrosLista(ListaCarros *carrosIni)
{
    int cont=0;
    while(carrosIni){
        cont++;
        carrosIni = carrosIni->prox;
    }
    return cont;
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
    return;
}

int verificarQuantidadeCarros(NoCarro *topo)
{
    int cont=0;
    while(topo){
        cont++;
        topo = topo->prox;
    }
    return cont;
}

int main(void)
{
    arquivo = fopen("log1dia.txt", "a"); // Cria ou abre o arquivo de log.

    if(arquivo == NULL){ // Caso haja algum problema com o arquivo e ele não seja aberto, uma mensagem é exibida no console.
        printf("Ops! Erro ao tentar abrir o arquivo.");
        exit(1);
    }

    // CADASTRO DE FUNCIONÁRIOS

    char nome[50];
    int idade;

    for(int i = 0; i < QUANTIDADE_FUNCIONARIOS; i++){
        printf("Digite o nome do funcionario: ");
        scanf("%s",nome);
        printf("Digite a idade do funcionario: ");
        scanf("%d",&idade);
        cadastrarFuncionarios(&filaFuncionarios, nome, (i + 1), idade, arquivo);
    }

    imprimirFila(filaFuncionarios);

    fprintf(arquivo, "Ordenacao usada: ID;\n");
    fprintf(arquivo, "Abertura do estacionamento (lotacao maxima = %d).\n", TAMANHO_ESTACIONAMENTO);
    if (topo == NULL){
        fprintf(arquivo, "Estacionamento vazio!\n");
    }

    // COLOCAR CARROS NO ESTACIONAMENTO (PILHA)

    srand(time(NULL));
    for (int i=0; i<TAMANHO_ESTACIONAMENTO; i++){
        int custo = gerarValores();
        topo = empilharCarro(topo,i+1,custo,i+1,arquivo);
    }

    if (verificarQuantidadeCarros(topo) == TAMANHO_ESTACIONAMENTO){
        fprintf(arquivo, "Estacionamento lotado!\n");
    }

    imprimirPilha(topo);

    while(lucroEstacionamento < 300){

        printf("**********MENU PRINCIPAL - ESTACIONAMENTO**********\n");

        printf("1-Adicionar um carro\t2-Remover um carro\t3-Alterar ordem dos funcionarios\n");

        int opcao;

        scanf("%d",&opcao);
        if (opcao == 1){
            if (verificarQuantidadeCarros(topo) == TAMANHO_ESTACIONAMENTO){
                printf("Erro: o estacionamento esta lotado!\n");
            }else{
                adicionarCarro();
                imprimirPilha(topo);
            }
        }else if (opcao == 2){
            if (verificarQuantidadeCarros(topo) == 0){
                printf("Erro: o estacionamento esta vazio!\n");
            }else{
                removerCarro();
                imprimirPilha(topo);
            }
        }else if(opcao == 3){
            printf("1-Ordenar por ID\t2-Ordenar por idade\t3-Ordenar por nome\n");
            scanf("%d",&opcao);
            if (opcao == 1){
                ordenarFuncionariosID(filaFuncionarios);
            } else if(opcao == 2){
                ordenarFuncionariosIdade(filaFuncionarios);
            } else if(opcao == 3){
                ordenarFuncionariosNome(filaFuncionarios);
            }

            imprimirFila(filaFuncionarios);
        }else{
            printf("Erro: opcao invalida!\n");
        }
    }

    fprintf(arquivo, "Valor do portão obtido! (Total arrecadado: %d). Nenhum carro pode entrar!\n", lucroEstacionamento);
    printf("Valor do portao obtido! Nenhum carro pode entrar!\n");

    removerCarrosFechamento();

    fprintf(arquivo, "Estacionamento fechado!\n"); // Mensagem de fechamento do primeiro dia.

    // **********SEGUNDO DIA**********

    arquivoSimulacao = fopen("log2dia.txt", "a");


    // CADASTRO DOS FUNCIONARIOS

    NoFuncionario *novoFuncionario;

    for(int i = 0; i < QUANTIDADE_FUNCIONARIOS; i++){
        novoFuncionario = removerDaFila(&filaFuncionarios);
        cadastrarFuncionarios(&filaFuncionarios, novoFuncionario->nome, novoFuncionario->id, novoFuncionario->idade, arquivoSimulacao);
    }

    imprimirFila(filaFuncionarios);

    fprintf(arquivoSimulacao, "Ordenacao usada: ID;\n");
    fprintf(arquivoSimulacao, "Abertura do estacionamento (lotacao maxima = %d).\n", TAMANHO_ESTACIONAMENTO);

    if (carrosIni == NULL){
        fprintf(arquivoSimulacao, "Estacionamento vazio!\n");
    }

    int quantidadeCarros; 
    
    while (carrosSimulacao)
    {
        if(strcmp(carrosSimulacao->atividade, "entrou") == 0){
            quantidadeCarros = contarCarros(carrosIni);
            insereLista(&carrosIni, &carrosFim, quantidadeCarros, carrosSimulacao->placa, carrosSimulacao->valor, arquivoSimulacao);
        }else if(strcmp(carrosSimulacao->atividade, "deve sair") == 0){
            int tamanhoIni;
            int tamanhoFim;

            tamanhoFim = verificarTamanhoFim(&carrosSimulacao, carrosSimulacao->placa);
            tamanhoIni = verificarTamanhoIni(&carrosSimulacao, carrosSimulacao->placa);

            if(tamanhoIni <= tamanhoFim){
                removerListaIni(carrosSimulacao->placa, carrosSimulacao->valor);
            }else{
                removerListaFim(carrosSimulacao->placa, carrosSimulacao->valor);
            }

        }
    }
    
    fclose(arquivo); //fechar arquivo
    
    return 0;

}

