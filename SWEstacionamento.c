#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BibliotecaSW.h"
#include <time.h>
#define qtdFuncionarios 3
#define tamEstacionamento 9
//

//TODO: NUMERO DE FUNCIONARIOS: 3         NUMERO TOTAL DE CARROS: 9
//TODO: a lista de carros, no dia 2, deverá ser duplamente encadeada

int main()
{
    //Criando ponteiro para arquivo
    FILE *arquivo;      // criando a variável ponteiro para o arquivo
    arquivo = fopen("log.txt", "a");   //abrindo o arquivo
    
    if (arquivo == NULL){    //Testando a abertura do arquivo
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }
    //Fim da criacao do ponteiro para arquivo

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
    for (int i=0; i<tamEstacionamento; i++){
        char id = (char)((int)'A' + i); 
        topo = empilharCarro(topo,id,i+1,arquivo);  
    }

    //imprimirPilha(topo);

    fclose(arquivo); //fechar arquivo

    //leitura do conteudo de log.txt e impressao no output
    int c;
    FILE *arquivoLeitura;
    arquivoLeitura = fopen("log.txt", "r");
    while ((c = getc(arquivoLeitura)) != EOF)
        putchar(c);
    fclose(arquivoLeitura);
    

    //imprimirFila(filaFunc); //imprimir fila de funcionarios
    

    
    return 0;
}
