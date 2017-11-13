NPES=2
DIM=4

all:
	@mpicc src/main.c src/gauss.c src/matrix.c -o bin/gaussjordan -lm -fopenmp -Wall 

clean:
	@rm -rf *.o *~ bin/*

run:
	@mpirun -np ${NPES} ./bin/gaussjordan ${DIM}
	$(info NPES="$(NPES)", DIM="$(DIM)")
