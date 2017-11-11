all:
	@mpicc src/main.c src/gauss.c src/matrix.c -o bin/gaussjordan -fopenmp -Wall 

clean:
	@rm -rf *.o *~ bin/*

run:
	@mpirun -np 2 ./bin/gaussjordan ${DIM}


