## Trabalho para disciplina Sistemas Operacionais, onde foi feita uma simulação de como funcionam os seguintes algoritmos de escalonamento de processos 

### FCFS ( First come, first served )
No FCFS, os processos são ordenados pela sua ordem de chegada, e são executados assim que chegam na fila de prontos e a CPU está livre.

### SJF ( Shortest job first )
No SJF sem preempção, os processos são ordenados a partir de um tempo de execução estimado, onde o processo com menor tempo de pico irá ter acesso prioritário a CPU, ele sempre atinge o melhor tempo de retorno médio possível. 

### RR ( Round Robin )
No Round Robin, existe uma parcela de tempo, chamada quantum, que representa quanto tempo o processo tem direito a CPU, dessa forma conseguimos um tempo de resposta bem melhor se comparado aos outros algoritmos.

## Entrada
A entrada é feita através de arquivo, composta por uma série de pares de números inteiros separados por um espaço em branco indicando o tempo de chegada e a duração de cada processo. A entrada termina com o fim do arquivo.
## Exemplo de entrada
```
0 20 
0 10 
4 6 
4 8
```

## Saída
A saída é composta por linhas contendo a sigla de cada um dos três algoritmos e os valores das três métricas solicitadas. 
Cada linha apresenta a sigla do algoritmo e os valores médios (com uma casa decimal) para tempo de retorno, tempo de resposta e tempo de espera, respectivamente, separados por um espaço em branco.
## Exemplo de saída
```
FCFS 30.5 19.5 19.5
SJF 21.5 10.5 10.5
RR 31.5 2.0 20.5
```


