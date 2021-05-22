#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BibliotecaSW.h"
#define qtdFuncionarios 2
#define tamEstacionamento 4

//TODO: dentro do while referente ao montante do estacionamento, deverá ser criado funcões ao invés de ficar repetindo o codigo

FILE *arquivo;      // criando a variável ponteiro para o arquivo
int montanteEstacionamento = 0; // montante do arrecadado no estacionamento.
NoCarro *rua = NULL; //estrutura que vai servir para armazenar os carros temporariamente na rua na hora das manobras
NoCarro *topo = NULL;  //estrutura do estacionamento (pilha)
NoFuncionario *filaFunc = NULL; //estrutura dos funcionarios (fila)


void removerCarro()
{
    // -------------- Colocar carros na rua --------------
    int resposta;
    printf("Deseja retirar qual carro?\n");
    scanf("%d", &resposta);
    int estadia = checarValorEstadia(topo,resposta);
    fprintf(arquivo, "Carro %d devera sair (Estadia = %d)\n", resposta, estadia);

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
    //int id = gerarID(topo,tamEstacionamento);
    int id;
    printf("Digite a placa do carro: ");
    scanf("%d",&id);
    int totalCarros = checarQtdCarros(topo);
    topo = empilharCarro(topo,id,custo,totalCarros,arquivo); //gerarId retornará um id que não exista ainda na pilha
}

void ordenarFuncionarios()
{
    //TODO: falta criar essa funcao
}

int main()
{
    arquivo = fopen("log.txt", "a");   //abrindo o arquivo
    
    if (arquivo == NULL){    //Testando a abertura do arquivo
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }

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
    }

    if (checarQtdCarros(topo) == tamEstacionamento) 
        fprintf(arquivo, "Estacionamento lotado!\n");

    imprimirPilha(topo);


    while(montanteEstacionamento < 300){

        if (checarQtdCarros(topo) == tamEstacionamento){
            printf("Estacionamento lotado! Retire um carro!\n");
            removerCarro();
            imprimirPilha(topo);
        }
        else if (checarQtdCarros(topo) == 0){
            printf("Estacionamento vazio! Adicione um carro!\n");
            adicionarCarro();
            imprimirPilha(topo);
        }
        else{
            printf("Tecle 1 para adicionar e 2 para retirar um carro\n");
            int op;
            scanf("%d",&op);
            if (op == 1){
                adicionarCarro();
            }else{
                removerCarro();
            }
            imprimirPilha(topo);
        }
    }

    fprintf(arquivo, "Valor do portão obtido! (Total arrecadado: %d). Nenhum carro pode entrar!\n", montanteEstacionamento);

    //imprimirPilha(topo);
    //imprimirFila(filaFunc);

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

