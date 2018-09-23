all: escalonamentoSO.o

escalonamentoSO.o: escalonamentoSO.cpp
	@g++ -o escalonamentoSO.o escalonamentoSO.cpp