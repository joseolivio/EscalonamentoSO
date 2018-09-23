#include <iostream>
#include <fstream> // biblioteca para manipulação de arquivos
#include <cstdlib> // biblioteca padrão da linguagem C
#include <queue>
#include <iomanip>      // std::setprecision
using namespace std;
#define quantum 2


void FCFS(int numProcessos, int tChegada[], int tPico[]) {
    // saída => FCFS tempo de retorno tempo de resposta tempo de espera
    int tRetorno[numProcessos+1]; // tempo que levou para executar o processo por completo ( desde que entrou ) [ turn around time ]
    int tEspera[numProcessos+1];  // tempo que esperou na fila de prontos. [ waiting time ]
    int tPicoFCFS[numProcessos+1];
    int tChegadaFCFS[numProcessos+1];
    int tResposta[numProcessos+1]; // tempo que esperou até dar a primeira resposta ( entrar na CPU ) [ response time ]
    int tempoCPU[numProcessos+1]; // esse array vai marcar o tempo exato em que o processo i entra na CPU.
    double mediatRetorno = 0;
    double mediatEspera = 0;
    double mediatResposta = 0;

    for(int i=1;i<=numProcessos;i++) {    // copia o array com os tempos de chegada para esse novo array.
        tChegadaFCFS[i] = tChegada[i];
    }
    for(int i=1;i<=numProcessos;i++) {    // copia o array com os tempos de chegada para esse novo array.
        tPicoFCFS[i] = tPico[i];
    }

    int i,j,menor;
    // ORDENO A ENTRADA DE ACORDO COM O SEU TEMPO DE PICO, PARA TRATAR O PROBLEMA DE ENTRADAS ( DO ARQUIVO ) NÃO ESTAREM ORDENADAS!!
    // SELECTION SORT:

    for(i=1;i<=numProcessos;i++){

        menor=i;
        for(j=i+1;j<=numProcessos;j++){
            if(tChegadaFCFS[j] < tChegadaFCFS[menor]){
                menor = j;
            }
        }
        swap(tPicoFCFS[menor],tPicoFCFS[i]);
        swap(tChegadaFCFS[menor],tChegadaFCFS[i]);

    }   // fim da ordenação

    tempoCPU[1] = tChegadaFCFS[1]; // o primeiro processo não espera nada para utilizar a CPU
    tEspera[1] = 0; // o primeiro processo não espera nada para utilizar a CPU
    tResposta[1] = 0; // tempo no qual o processo ENTROU na CPU.

    for(int i=2;i <= numProcessos;i++) {     // vou percorrer todos os processo com exceção do primeiro que tem seu tempo de espera igual a zero.

        tempoCPU[i] = tempoCPU[i-1] + tPicoFCFS[i-1]; // essa variavel vai marcar o tempo exato em que o processo i entra na CPU.
        tResposta[i] = tempoCPU[i] - tChegadaFCFS[i];
        tEspera[i] = tempoCPU[i] - tChegadaFCFS[i]; // como não há preempção, o tempo de espera é a subtração entre o tempo que entrou na CPUI e o tempo que chegou.

        if(tEspera[i] < 0) {  // Se o tempo de espera deu negativo é porque quando o processo entrou a CPU já estava livre, ou seja, não precisou esperar...
            tEspera[i] = 0;  // logo seu tempo de espera foi zero, exemplo: Ultimo processo deixou a CPU no tempo 5 e esse processo chegou no tempo 15...
        }

        }

    for(int i=1;i<=numProcessos;i++) {
        tRetorno[i] = tPicoFCFS[i] + tEspera[i]; // como não há preemção, o tempo de retorno é igual o tempo que ele esperou até usar a cpu + o tempo que ele usou a CPU.
    }

    // tudo calculado, agora tirar as médias:

    for(int i=1;i<=numProcessos;i++) {
     /*   cout << "Tempo de retorno de " << i << " :" << tRetorno[i] << endl;
        cout << "Tempo de resposta de " << i << " :" << tResposta[i] << endl;
        cout << "Tempo de espera de " << i << " :" << tEspera[i] << endl;  */

        mediatRetorno += tRetorno[i];   // vou somando todos os valores para obter a soma total dos tempos de retorno
        mediatEspera += tEspera[i];     // vou somando todos os valores para obter a soma total dos tempos de espera
        mediatResposta += tResposta[i]; // vou somando todos os valores para obter a soma total dos tempos de resposta

    }
        mediatRetorno = mediatRetorno/numProcessos; // calculo a média do tempo de retorno
        mediatEspera = mediatEspera/numProcessos;   // calculo a média do tempo de espera
        mediatResposta = mediatResposta/numProcessos; // calculo a média do tempo de resposta
        cout << fixed;
        cout << setprecision(1);
        cout << "FCFS " << mediatRetorno << " " <<  mediatResposta << " " << mediatEspera << endl;


} // fim do FCFS
void SJF(int numProcessos, int tChegada[], int tPico[]) {
    int tRetorno[numProcessos+1]; // tempo que levou para executar o processo por completo ( desde que entrou ) [ turn around time ]
    int tEspera[numProcessos+1];  // tempo que esperou na fila de prontos. [ waiting time ]
    int tPicoSJF[numProcessos+1];
    int tResposta[numProcessos+1]; // tempo que esperou até dar a primeira resposta ( entrar na CPU ) [ response time ]
    int tempoCPU[numProcessos+1]; // esse array vai marcar o tempo exato em que o processo i entra na CPU.
    int tChegadaResta[numProcessos+1]; // esse array vai me ajudar a calcular o tempo de espera.
    int tempoAtual = 0;         // indica qual o tempo atual, funciona como uma linha do tempo
    double mediatRetorno = 0;   // variável para calcular a média dos tempos.
    double mediatEspera = 0;    // variável para calcular a média dos tempos.
    double mediatResposta = 0;  // variável para calcular a média dos tempos.
    int quantosFaltam = numProcessos; // variável para saber quantos processos ainda faltam usar a CPU.

    for(int i=1;i<=numProcessos;i++) {    // copia o array com os tempos de chegada para esse novo array.
        tChegadaResta[i] = tChegada[i];
    }
    for(int i=1;i<=numProcessos;i++) {    // copia o array com os tempos de chegada para esse novo array.
        tPicoSJF[i] = tPico[i];
    }
/* Utilizo o algoritmo de ordenação selection sort ( ordenação por seleção ) para ordenar os processos de acordo com o seu tempo de pico, cada processo é um índice (i)
no programa, consigo ordenar levando em consideração o tempo de pico mas mantendo os pares de entrada.. ou seja, o tchegada também vai ser trocado de acordo c/ o
tempo de pico, exemplo: processo "3" tem o tempo de pico menor que o processo "1", então tanto o tPico[] e o tChegada[] serão alterados, o "3" vai virar "1" e vice-versa */

    int i,j,menor;

    for(i=1;i<=numProcessos;i++){

        menor=i;
        for(j=i+1;j<=numProcessos;j++){
            if(tPicoSJF[j] < tPicoSJF[menor]){
                menor = j;
            }
        }
        swap(tPicoSJF[menor],tPicoSJF[i]);
        swap(tChegadaResta[menor],tChegadaResta[i]);

    }   // fim da ordenação

 /*        for(int i=1;i<=numProcessos;i++) {
        cout << "APÓS ORDENAR, CONFIRA OS PARES-> valores do tChegada: " << i << " " << tChegada[i] << endl;
        cout << "APÓS ORDENAR, CONFIRA OS PARES-> valores do tPico: " << i << " " <<  tPico[i] << endl;
}*/

    while(quantosFaltam>0) {    // executo esse laço até todos os processos serem executados ( entrarem na CPU ).
        for(int i=1;i<=numProcessos;i++) {  // vou passar por todos os processos, como já estão ordenados, os de menor tempo de pico tem prioridade.
            if(tChegadaResta[i] <= tempoAtual) { // checo se ele já chegou, utilizando a variável tempoAtual, se ele chegou, significa que deve ter acesso a CPU, pois já garantimos que o seu tempo de pico é menor.
                tempoCPU[i] = tempoAtual;   // aqui ele "entra" na CPU, realizo vários cálculos como os tempos de espera,resposta e retorno.
                tEspera[i] = tempoCPU[i] - tChegadaResta[i];
                tResposta[i] = tempoCPU[i] - tChegadaResta[i];
                tRetorno[i] = tPicoSJF[i] + tEspera[i];
                tempoAtual += tPicoSJF[i];         // o tempo "passa" após a execução deste processo.
                tChegadaResta[i] = 99999999;         // como esse processo já foi executado, eu artificialmente jogo seu tempo de chegada para um valor alto, para não ter riscos de executar novamente.
             //   cout << "Processo " << i << " entrou na CPU!!" << "Tempos atrelados: " << tEspera[i] << " " << tResposta[i] << " " << tRetorno[i] << endl;

                if(tEspera[i] < 0) {  // Se o tempo de espera deu negativo é porque quando o processo entrou a CPU já estava livre, ou seja, não precisou esperar...
                    tEspera[i] = 0;  // logo seu tempo de espera foi zero, exemplo: Ultimo processo deixou a CPU no tempo 5 e esse processo chegou no tempo 15...
                }
                if(tResposta[i] < 0) {  // Se o tempo de espera deu negativo é porque quando o processo entrou a CPU já estava livre, ou seja, não precisou esperar...
                    tResposta[i] = 0;  // logo seu tempo de espera foi zero, exemplo: Ultimo processo deixou a CPU no tempo 5 e esse processo chegou no tempo 15...
                }
                quantosFaltam--;            // decremento variavel que indica quantos processos faltam serem executados.
                i=0;                        // zero o indice do for para percorrer todos os processos novamente, mas apenas os que não foram executados ainda serão levados em consideração.
            }

        }
    }

    for(int i=1;i<=numProcessos;i++) {
    /*    cout << "Tempo de retorno de " << i << " :" << tRetorno[i] << endl;
        cout << "Tempo de resposta de " << i << " :" << tResposta[i] << endl;
        cout << "Tempo de espera de " << i << " :" << tEspera[i] << endl;*/

        mediatRetorno += tRetorno[i];   // vou somando todos os valores para obter a soma total dos tempos de retorno
        mediatEspera += tEspera[i];     // vou somando todos os valores para obter a soma total dos tempos de espera
        mediatResposta += tResposta[i]; // vou somando todos os valores para obter a soma total dos tempos de resposta

    }
        mediatRetorno = mediatRetorno/numProcessos; // calculo a média do tempo de retorno
        mediatEspera = mediatEspera/numProcessos;   // calculo a média do tempo de espera
        mediatResposta = mediatResposta/numProcessos; // calculo a média do tempo de resposta

    cout << "SJF " << mediatRetorno << " " <<  mediatResposta << " " << mediatEspera << endl;

}   // fim do SJF
void RR(int numProcessos, int tChegada[], int tPico[]) {
    int tRetorno[numProcessos+1]; // tempo que levou para executar o processo por completo ( desde que entrou ) [ turn around time ]
    int tEspera[numProcessos+1] = {};  // tempo que esperou na fila de prontos. [ waiting time ]
    int tResposta[numProcessos+1]; // tempo que esperou até dar a primeira resposta ( entrar na CPU ) [ response time ]
    int tChegadaRest[numProcessos+1]; // esse array vai me ajudar a calcular o tempo de espera.
    int tPicoRest[numProcessos+1]; // esse array vai me mostrar quanto tempo ainda falta para os processos executarem
    int tQueSaiu[numProcessos+1] = {};
    int tQueEntrou[numProcessos+1] = {};
    int tempoAtual = 0;         // indica qual o tempo atual, funciona como uma linha do tempo
    bool toNaFila[numProcessos+1] = {};
    bool jaExecutou[numProcessos+1] = {};
    bool primeiraVez[numProcessos+1] = {};
    double mediatRetorno = 0;   // variável para calcular a média dos tempos.
    double mediatEspera = 0;    // variável para calcular a média dos tempos.
    double mediatResposta = 0;  // variável para calcular a média dos tempos.
    int quantosFaltam = numProcessos; // variável para saber quantos processos ainda faltam usar a CPU.
    queue <int> fila;
   // list<int> minhaLista(numProcessos,0);
   //list<int>::iterator it = myList.begin();



    for(int i=1;i<=numProcessos;i++) {    // copia o array com os tempos de pico para esse novo array.
    tPicoRest[i] = tPico[i];
    }

    for(int i=1;i<=numProcessos;i++) {    // copia o array com os tempos de chegada para esse novo array.
    tChegadaRest[i] = tChegada[i];
    }

    int i,j,menor;
    // ORDENO A ENTRADA DE ACORDO COM O SEU TEMPO DE PICO, PARA TRATAR O PROBLEMA DE ENTRADAS ( DO ARQUIVO ) NÃO ESTAREM ORDENADAS!!
    // SELECTION SORT:

    for(i=1;i<=numProcessos;i++){

        menor=i;
        for(j=i+1;j<=numProcessos;j++){
            if(tChegadaRest[j] < tChegadaRest[menor]){
                menor = j;
            }
        }
        swap(tPicoRest[menor],tPicoRest[i]);
        swap(tChegadaRest[menor],tChegadaRest[i]);

    }   // fim da ordenação


    while(quantosFaltam>0)  {
        for(int i=1;i<=numProcessos;i++) {  // vou passar por todos os processos.
                for(int j=1;j<=numProcessos;j++){
                    if(j==i && primeiraVez[j] == 0){
                        if((tChegada[j] <= tempoAtual) && (toNaFila[j] == 0) && (jaExecutou[j] == 0)){

                            fila.push(j);
                           // cout << "Processo(ID): " << j << " entrei na FILA PELO PRIMEIRO FOR" << endl;
                            toNaFila[j] = 1;
                        }
                    }
                }
                for(int j=1;j<=numProcessos;j++){
                    if(j!=i && primeiraVez[j] == 0){
                        if((tChegada[j] <= tempoAtual) && (toNaFila[j] == 0) && (jaExecutou[j] == 0)){

                            fila.push(j);
                            //cout << "Processo(ID): " << j << " entrei na FILA PELO SEGUNDO FOR" << endl;
                            toNaFila[j] = 1;
                        }
                    }
                }
                for(int j=1;j<=numProcessos;j++){
                        if(j!=i){
                                if((tChegada[j] <= tempoAtual) && (toNaFila[j] == 0) && (jaExecutou[j] == 0)){

                                        fila.push(j);
                                        //cout << "Processo(ID): " << j << " entrei na FILA PELO SEGUNDO FOR" << endl;
                                        toNaFila[j] = 1;
                                }
                        }
                }


                if(fila.front() == i && jaExecutou[i] == 0) {
                //cout << "Processo(ID): " << i << " entrei na CPU" << endl;
                // se for a primeira vez do processo na cpu, calculo tempo de resposta.
                        if(primeiraVez[i]==0) {
                            //cout << "Calculei tempo de resposta, processo(id): " << i << " tempoAtual: " << tempoAtual << " tChegada: " << tChegada[i] << endl;
                            tResposta[i] = tempoAtual - tChegada[i];
                            primeiraVez[i] = 1;
                        }



                if(tPicoRest[i]>quantum){
                   // cout << "Entrei aqui (i):" << i << endl;
                    tEspera[i] += (tempoAtual - tChegadaRest[i]); // Tempo que entrou - tempo que saiu da ultima vez!
                    tempoAtual += quantum;
                    tPicoRest[i] -= quantum;
                    fila.pop();
                    toNaFila[i] = 0;
                    tChegadaRest[i] = tempoAtual;
                }
                else if(tPicoRest[i]==quantum) {
                    //cout << "Processo(ID): " << i << " finalizei exec. na CPU" << endl;
                    tEspera[i] += (tempoAtual - tChegadaRest[i]); // Tempo que entrou - tempo que saiu da ultima vez!
                    tempoAtual += quantum;
                    tPicoRest[i] = 0;
                    tRetorno[i] = tempoAtual - tChegada[i];
                    jaExecutou[i] = 1;
                    quantosFaltam--;
                    fila.pop();
                    toNaFila[i] = 0;
                    tChegadaRest[i] = tempoAtual;

                }
                    else {
                        //cout << "Processo(ID): " << i << " finalizei exec. na CPU" << endl;
                        tEspera[i] += (tempoAtual - tChegadaRest[i]); // Tempo que entrou - tempo que saiu da ultima vez!
                        tempoAtual++;
                        tPicoRest[i] = 0;
                        tRetorno[i] = tempoAtual - tChegada[i];
                        quantosFaltam--;
                        jaExecutou[i] = 1;
                        fila.pop();
                        toNaFila[i] = 0;
                        tChegadaRest[i] = tempoAtual;

                    }
                }
        }
    }


    for(int i=1;i<=numProcessos;i++){

            if(tEspera[i] < 0) {  // Se o tempo de espera deu negativo é porque quando o processo entrou a CPU já estava livre, ou seja, não precisou esperar...
                    tEspera[i] = 0;  // logo seu tempo de espera foi zero, exemplo: Ultimo processo deixou a CPU no tempo 5 e esse processo chegou no tempo 15...
                }
            if(tResposta[i] < 0) {  // Se o tempo de espera deu negativo é porque quando o processo entrou a CPU já estava livre, ou seja, não precisou esperar...
                    tResposta[i] = 0;  // logo seu tempo de espera foi zero, exemplo: Ultimo processo deixou a CPU no tempo 5 e esse processo chegou no tempo 15...
                }

    }


    for(int i=1;i<=numProcessos;i++) {
       // cout << "Tempo de retorno de " << i << " :" << tRetorno[i] << endl;
       // cout << "Tempo de resposta de " << i << " :" << tResposta[i] << endl;
       // cout << "Tempo de espera de " << i << " :" << tEspera[i] << endl;

        mediatRetorno += tRetorno[i];   // vou somando todos os valores para obter a soma total dos tempos de retorno
        mediatEspera += tEspera[i];     // vou somando todos os valores para obter a soma total dos tempos de espera
        mediatResposta += tResposta[i]; // vou somando todos os valores para obter a soma total dos tempos de resposta

    }
        mediatRetorno = mediatRetorno/numProcessos; // calculo a média do tempo de retorno
        mediatEspera = mediatEspera/numProcessos;   // calculo a média do tempo de espera
        mediatResposta = mediatResposta/numProcessos; // calculo a média do tempo de resposta

    cout << "RR " << mediatRetorno << " " <<  mediatResposta << " " << mediatEspera << endl;


} // fim do RR



int main()

{
    char nomeArquivo[80]; // essa variável indica qual o nome do arquivo que será aberto
    int tChegada[50] = {0} ;
    int tPico[50] = {0} ;
    int idProcesso = 1;

    ifstream ifstreamArquivo;      // objeto da biblioteca ifstream, usado para manipular funções dessa biblioteca
    cout << "Insira o nome do arquivo de entrada:" << endl;
    cin >> nomeArquivo;

    ifstreamArquivo.open(nomeArquivo);    // abre o arquivo, cujo nome foi inserido pelo usuário

        while(ifstreamArquivo >> tChegada[idProcesso] >> tPico[idProcesso]) {
                idProcesso++;
}
    int numProcessos = idProcesso - 1;

    cout << "Numero de processos: " << numProcessos << endl;

    cout << "Processo:  " << "T.Chegada: " << "T.Pico: " << endl;

    for(int i=1;i<=numProcessos;i++) {
        cout <<  i << "          " << tChegada[i] << "            " << tPico[i] << endl;
        cout << endl;

}
    for(int i=1;i<=numProcessos;i++){
        if(tPico[i] < 0){
            cout << "Entrada negativa detectada, por favor, insira apenas valores positivos!" << endl;
            exit(1);

        }
        if(tChegada[i] < 0){
            cout << "Entrada negativa detectada, por favor, insira apenas valores positivos!" << endl;
            exit(1);
        }
    }

    FCFS(numProcessos,tChegada,tPico);
    SJF(numProcessos,tChegada,tPico);
    RR(numProcessos,tChegada,tPico);

} // fim do main
