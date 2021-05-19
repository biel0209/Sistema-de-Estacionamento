#include <stdio.h>
#include <stdlib.h>
#include "BibliotecaSW.h"
#define qtdFuncionarios 3
#define tamEstacionamento 9

//TODO: NUMERO DE FUNCIONARIOS: 3         NUMERO TOTAL DE CARROS: 9



int main()
{
    char texto_Arquivo[100][100];
    
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
    

    fprintf(arquivo, "Abertura do estacionamento (lotacao maxima = %d).\n", tamEstacionamento);

    fprintf(arquivo, "Estacionamento vazio!\n");

    fclose(arquivo); //fechar arquivo


    //Procedimento pra imprimir as informacoes contidas no arquivo log
    FILE *arquivoLeitura;
    arquivoLeitura = fopen("log.txt", "r");
 
    for(int i=0; i<100; i++){                                         //laço referente a quantidade de linhas
        while( fgets(texto_Arquivo[i], 100, arquivoLeitura) != NULL ) //laço pra imprimir cada string, ou seja, cada linha
            printf("%s", texto_Arquivo[i]);
    }


    //imprimirFila(filaFunc); //imprimir fila de funcionarios


    
    return 0;
}
