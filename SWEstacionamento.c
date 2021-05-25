#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BibliotecaSW.h"
#define qtdFuncionarios 2
#define tamEstacionamento 4

FILE *arquivo;      // criando a variável ponteiro para o arquivo do log do primeiro dia
FILE *arquivoDia2;      // criando a variável ponteiro para o arquivo do log do segundo dia
int montanteEstacionamento = 0; // montante do arrecadado no estacionamento.
NoCarro *rua = NULL; //estrutura que vai servir para armazenar os carros temporariamente na rua na hora das manobras
NoCarro *topo = NULL;  //estrutura do estacionamento (pilha)
RegCarro *regCarros = NULL; //estrutura pra registrar a sequencia de acontecimentos. Servira para seguir a mesma rotina no segundo dia
ListaCarro *listaCarros = NULL; //estrutura dos carros do segundo dia (referencia do começo da lista)
ListaCarro *listaCarrosFim = NULL; //estrutura dos carros do segundo dia (referencia do final da lista)
NoFuncionario *filaFunc = NULL; //estrutura dos funcionarios (fila)


void removerCarro()
{
    // -------------- Colocar carros na rua --------------
    int resposta;
    printf("Deseja retirar qual carro?: ");
    scanf("%d", &resposta);
    int estadia = checarValorEstadia(topo,resposta);
    fprintf(arquivo, "Carro %d devera sair (Estadia = %d)\n", resposta, estadia);

    insereNaListaReg(&regCarros, resposta, estadia, "devera sair");
    

    montanteEstacionamento += estadia;   //adicionar a estadia do carro no lucro do estacionamento
    
    NoCarro* tempAux;   
    int cont = 0;

    NoFuncionario* func_trabalhando;
    //esse laço será referente até o momento que o carro desejado será retirado do estacionamento
    for (int i = 0; i < tamEstacionamento; i++) {
        func_trabalhando = removerDaFila(&filaFunc); // funcionario que desempilha o carro.
        tempAux = desempilharCarro(&topo);               // desempilha os carros
        fprintf(arquivo, "-----Carro %d retirado pelo funcionario %s!\n", tempAux->placa,func_trabalhando->nome);
        if (tempAux->placa != resposta) {  //enquanto nao for o carro que deve ser retirado, empilhe na rua
            rua = empilharCarroRua(rua, tempAux->placa, tempAux->valor);
            cont++;
        }else{
            cadastrarFuncionarios2(&filaFunc,func_trabalhando->nome, func_trabalhando->id, func_trabalhando->idade);
            break;
        }
        cadastrarFuncionarios2(&filaFunc,func_trabalhando->nome, func_trabalhando->id, func_trabalhando->idade);
    }

    //procedimento para retornar todos os carros da rua de volta para o estacionamento
    for (int i = 0; i < cont; i++) {
        func_trabalhando = removerDaFila(&filaFunc); // funcionario que desempilha o carro.
        tempAux = desempilharCarro(&rua);
        topo = empilharCarro2(topo, tempAux->placa, tempAux->valor);
        fprintf(arquivo, "-----Carro %d voltou pelo funcionario %s!\n", tempAux->placa,func_trabalhando->nome);
        cadastrarFuncionarios2(&filaFunc,func_trabalhando->nome, func_trabalhando->id, func_trabalhando->idade);
    }

    fprintf(arquivo, "Carro %d saiu! Total = %d\n", resposta, checarQtdCarros(topo));
}

void adicionarCarro()
{
    int custo = gerarValores();
    int id;
    printf("Digite a placa do carro: ");
    scanf("%d",&id);
    int totalCarros = checarQtdCarros(topo);
    topo = empilharCarro(topo,id,custo,totalCarros+1,arquivo);
    insereNaListaReg(&regCarros, id, custo, "entrou");
}

void removerLista(int placa, int estadia)
{
    int tamInicio = checarTamInicio(listaCarros, placa,estadia);
    int tamFim = checarTamFim(listaCarrosFim,placa,estadia);
   
    fprintf(arquivoDia2, "Carro %d devera sair\n",placa);

    removerDaListaInicio(&listaCarros,placa, estadia,rua,arquivoDia2);
    /*
    if (tamInicio <= tamFim){
        removerDaListaInicio(&listaCarros,placa, estadia,rua,arquivoDia2);
    }else{
        removerDaListaFim(listaCarrosFim,placa, estadia);
    } */

    fprintf(arquivoDia2, "Carro %d saiu. Total = %d\n",placa,checarQtdCarrosLista(listaCarros));
}


int main()
{
    arquivo = fopen("log.txt", "a");   //abrindo o arquivo

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

    imprimirFila(filaFunc);

    fprintf(arquivo, "Ordenacao usada: ID;\n");
    fprintf(arquivo, "Abertura do estacionamento (lotacao maxima = %d).\n", tamEstacionamento);
    if (topo == NULL)
        fprintf(arquivo, "Estacionamento vazio!\n");

    //---------Colocando carros na pilha do estacionamento--------
    srand(time(NULL));
    for (int i=0; i<tamEstacionamento; i++){
        int custo = gerarValores();
        topo = empilharCarro(topo,i+1,custo,i+1,arquivo);
        insereNaListaReg(&regCarros, i+1, custo, "entrou");
    }

    if (checarQtdCarros(topo) == tamEstacionamento) 
        fprintf(arquivo, "Estacionamento lotado!\n");

    imprimirPilha(topo);


    while(montanteEstacionamento < 300){

        printf("--------------------------MENU--------------------------\n");
        printf("1-Adicionar um carro\t2-Remover um carro\t3-Alterar ordem dos funcinoarios\n");
        int op;
        scanf("%d",&op);
        if (op == 1){
            if (checarQtdCarros(topo) == tamEstacionamento){
                printf("Erro: o estacionamento esta lotado!\n");
            }else{
                adicionarCarro();
                imprimirPilha(topo);
            }
        }else if (op==2){
            if (checarQtdCarros(topo) == 0){
                printf("Erro: o estacionamento esta vazio!\n");
            }else{
                removerCarro();
                imprimirPilha(topo);
            }
        }else if(op==3){
            printf("1-Ordenar por ID\t2-Ordenar por nome\t3-Ordenar por idade\n");
            scanf("%d",&op);
            if (op==1){
                ordenarFuncionariosID(filaFunc);
                fprintf(arquivo, "Ordenacao usada: ID;\n");
            }
            else if(op==2){
                ordenarFuncionariosNome(filaFunc);
                fprintf(arquivo, "Ordenacao usada: Nome;\n");
            }
            else if(op==3){
                ordenarFuncionariosIdade(filaFunc);
                fprintf(arquivo, "Ordenacao usada: Idade;\n");
            }
            imprimirFila(filaFunc);
        }else{
            printf("Erro: opcao invalida!\n");
        }
    }

    fprintf(arquivo, "Valor do portão obtido! (Total arrecadado: %d). Nenhum carro pode entrar!\n", montanteEstacionamento);

    printf("Valor do portao obtido! Nenhum carro pode entrar!\n");

    while (checarQtdCarros(topo) != 0){
        removerCarro();
    }

    fprintf(arquivo, "Fechamento do estacionamento.\n");

    imprimirListaReg(regCarros);

    //######################################  SEGUNDO DIA   #########################################
    
    arquivoDia2 = fopen("log2.txt","a");

    //Procedimento pra receber do usuario informacoes sobre o funcionario e cadastra-los
    NoFuncionario *funcRemovido;
    for (int i=0; i<qtdFuncionarios; i++){
        funcRemovido = removerDaFila(&filaFunc);
        cadastrarFuncionarios(&filaFunc, funcRemovido->nome,funcRemovido->id,funcRemovido->idade,arquivoDia2);
    }
    //Fim do cadastramento

    imprimirFila(filaFunc);

    fprintf(arquivoDia2, "Ordenacao usada: ID;\n");   //TODO: QUAL ORDENACAO ESTA SENDO USADA?
    fprintf(arquivoDia2, "Abertura do estacionamento (lotacao maxima = %d).\n", tamEstacionamento);
    if (topo == NULL)
        fprintf(arquivoDia2, "Estacionamento vazio!\n");

    
    while (regCarros){
        if ( strcmp(regCarros->acao, "entrou") == 0 ){
            insereNaLista(&listaCarros, &listaCarrosFim, regCarros->placa, regCarros->valor, checarQtdCarrosLista(listaCarros)+1, arquivoDia2);
        }else if ( strcmp(regCarros->acao, "devera sair") == 0 ){
            removerLista(regCarros->placa, regCarros->valor);
        }
        regCarros = regCarros->prox;
    }

    imprimirLista(listaCarros);
  
    //imprimirLista2(listaCarrosFim); //testar a iteracao atraves do anterior, começando pelo final da lista
    


    fclose(arquivo); //fechar arquivo

    /*leitura do conteudo de log.txt e impressao no output
    int c;
    FILE *arquivoLeitura;
    arquivoLeitura = fopen("log.txt", "r");
    while ((c = getc(arquivoLeitura)) != EOF)
        putchar(c);
    fclose(arquivoLeitura);
    */

    return 0;
}

