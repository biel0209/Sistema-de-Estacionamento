# Sistema-de-Estacionamento


Avaliacao da primeira unidade da disciplina de Estrutura de Dados com Rodolfo

//TODO: 1. Introdução

    O administrador de um estacionamento 24 horas vai utilizar o software SW de 
gerenciamento. No SW é possível realizar cadastro de funcionários (nome, ID, idade) e 
identificar os carros que estão no estacionamento (Placa sem regra). O espaço do 
estacionamento é muito limitado, sua lotação máxima é estabelecida previamente e só temos um 
portão tanto para entrada quanto para saída de carros. Dessa forma, os carros deverão ser 
organizados em uma única fileira, com a restrição de que o último carro que entrar será sempre 
o primeiro a sair.

Um cliente que desejar retirar seu carro terá que informar a placa dele. Ainda por conta 
do pequeno espaço, manobras fora do estacionamento (por exemplo, na rua) devem ser 
realizadas pelos funcionários. A escolha pelo funcionário para manobrar um carro deve seguir 
uma ordem estabelecida pelo administrador, que pode ser alterada a qualquer momento 
(podemos ter uma ordenação por ID, por idade ou por nome). O cliente entra no carro 
apenas quando este estiver do lado de fora do estacionamento e vai embora. Após a partida do 
carro procurado, os demais carros que voltarão para o estacionamento deverão obedecer a 
mesma sequência de antes. A economia de espaço deve ser espelhada na alocação de memória 
para todas as estruturas envolvidas neste projeto.

Ao considerar que a estadia do carro custe R$12,00 (doze reais) por hora, este deve ser 
o valor mínimo pago (ex: estadia de 2 segundos, pagamento de 12 reais). Para os casos de 
minutos excedentes, será cobrada uma hora inteira (ou seja, permanência de 1 hora e 1 minuto 
deverá ser cobrado o valor de 2 horas). O valor arrecadado servirá na construção de um novo 
portão (R$300,00 – trezentos reais), acreditando-se que um portão em cada extremidade da 
fileira de carros deverá acelerar o processo de retirada dos carros.

Uma vez que o valor de um novo portão for atingido, passamos para o esvaziamento do 
estacionamento, FECHAMENTO deste e construção do novo portão. Neste ponto, nenhum 
carro poderá entrar e todas as ações ocorridas no estacionamento deverão ser guardadas 
(sequência de entradas e saídas de carros, assim como sequência de funcionários solicitados).

Agora com 2 portões, os carros entrarão SEMPRE pelo portão antigo, mas a saída pode 
ser feita por qualquer um dos dois portões, incentivando a criatividade do gerente para obter 
processos mais rápidos (sempre respeitando as características das estruturas utilizadas).

Desenvolva o SW gerenciador, usando pelo menos duas listas diferentes (qualquer 
tipo de lista), uma pilha (além das listas anteriores), três funções de ordenação (por ID, pela 
idade e pelo nome) e um controlador que mostre todas as ações realizadas no estacionamento 
desde sua abertura até seu fechamento (momento até a arrecadação do valor do novo portão). A
quantidade de carros no estacionamento deverá ser mostrada sempre que um carro novo entra ou 
quando um carro sai do estacionamento em definitivo, seguindo o exemplo de log abaixo:

Funcionário de nome X1 (ID = 1; Idade = 34) cadastrado;
Funcionário de nome X2 (ID = 2; Idade = 57) cadastrado;
Abertura do estacionamento (lotação máxima = 3).
Estacionamento vazio!
Carro A entrou. Total = 1;
Carro B entrou. Total = 2;
Carro C entrou. Total = 3;
Estacionamento lotado!
Carro B deverá sair (estadia = 150 reais).
----- Carro C retirado pelo funcionário X1;
----- Carro B retirado pelo funcionário X2;
----- Carro C voltou.
Carro B saiu! Total = 2;
Carro C deverá sair (estadia = 170 reais).
----- Carro C retirado pelo funcionário X1;
Carro C saiu! Total = 1;
Valor do portão obtido! Nenhum carro pode entrar!
Carro A deverá sair (estadia = 220 reais).
----- Carro A retirado pelo funcionário X2;
Carro A saiu! Total = 0;
Fechamento do estacionamento.

O mesmo cenário ocorrido até o fechamento do estacionamento (a mesma sequência de 
carros que entraram, saíram e o mesmo grupo de funcionários) deverá ser repetido após a 
construção do novo portão, como uma simulação de comparação. Os acontecimentos do 
controlador podem ser gerados aleatoriamente ou por meio de menus. O log deverá ser 
impresso no fechamento do estacionamento e após a finalização da simulação.
Por fim, cada aluno deverá realizar algumas simulações no estacionamento (mínimo de 2), 
variando as quantidades de carros e funcionários, incluindo situações onde há mais carros que 
funcionários e vice-versa, a fim de comparar o número mínimo de manobras antes e pósconstrução do portão.

2. Pontuação

Esta unidade é composta por 2 listas de exercícios, totalizando 2 pontos na média e 1 
ponto extra, e o projeto prático apresentado neste documento valendo 8 pontos.3. Prazo
O trabalho deve feito individualmente. Todos os alunos deverão enviar os arquivos (códigos 
fonte e vídeos) até o dia 26/05/2021, na tarefa a ser criada no Google Classroom ou na 
atividade criada no SIGAA.

4. Política para Avaliação e Plágio

O aluno precisa fazer um vídeo de apresentação da avaliação. Este vídeo deve mostrar a 
explicação de todos os pontos requeridos no projeto, assim como a execução do programa. A 
falta de explicação de algum ponto será descontada na nota final e na falta do vídeo será 
atribuída a nota 0 (ZERO) ao Projeto prático.

Alunos que forem flagrados compartilhando código fonte com colegas ou utilizando código 
fonte de outros autores receberão nota 0 (ZERO)