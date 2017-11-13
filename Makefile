NPES=2
DIM=4
OPENMP=0

all:
	@mpicc src/main.c src/gauss.c src/matrix.c -o bin/gaussjordan -lm -fopenmp -Wall 

clean:
	@rm -rf *.o *~ bin/*

run:
	@mpirun -np ${NPES} ./bin/gaussjordan ${OPENMP} ${DIM} 
#	$(info NPES="$(NPES)", DIM="$(DIM), OPENMP="$(OPENMP)")
