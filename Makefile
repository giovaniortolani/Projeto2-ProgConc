NPES=2
DIM=4

all:
	@mpicc src/main.c src/gauss.c src/matrix.c -o bin/gaussjordan -lm -fopenmp -Wall 

clean:
	@rm -rf *.o *~ bin/*

run:
	@mpirun -np ${NPES} ./bin/gaussjordan ${DIM}
	$(info NPES="$(NPES)", DIM="$(DIM)")

runall:
	(mpirun -hostfile hosts -np 1 ./bin/gaussjordan 10; \
	mpirun -hostfile hosts -np 1 ./bin/gaussjordan 50; \
	mpirun -hostfile hosts -np 1 ./bin/gaussjordan 10000; \
	mpirun​ ​-hostfile hosts -np​ 2 ​./bin/gaussjordan 1000; \
	mpirun​ ​-hostfile hosts -np​ 2 ​./bin/gaussjordan 5000; \
	mpirun​ ​-hostfile hosts -np​ 2 ​./bin/gaussjordan 10000; \
	mpirun​ ​-hostfile hosts -np​ 4 ​./bin/gaussjordan 1000; \
	mpirun​ ​-hostfile hosts -np​ 4 ​./bin/gaussjordan 5000; \
	mpirun​ ​-hostfile hosts -np​ 4 ​./bin/gaussjordan 10000; \
	mpirun​ ​-hostfile hosts -np​ 8 ​./bin/gaussjordan 1000; \
	mpirun​ ​-hostfile hosts -np​ 8 ​./bin/gaussjordan 5000; \
	mpirun​ ​-hostfile hosts -np​ 8 ​./bin/gaussjordan 10000)


# TODO
# ordenar os valores caso ocorra troca de linha
# colocar openmp e passar o numero de threads pelo makefile