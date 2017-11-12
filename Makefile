NPES=2
DIM=4

all:
	@mpicc src/main.c src/gauss.c src/matrix.c -o bin/gaussjordan -fopenmp -Wall 

clean:
	@rm -rf *.o *~ bin/*

run:
	@mpirun -np ${NPES} ./bin/gaussjordan ${DIM}
	$(info NPES="$(NPES)", DIM="$(DIM)")
<<<<<<< HEAD
=======

>>>>>>> a33f31e5bb311297b29962dc0e285457aa937b11

