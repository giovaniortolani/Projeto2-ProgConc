
all: clean gauss

gauss:
	mpicc gauss.c -o gauss -Wall

clean:
	rm -rf gauss

run:
	mpirun -np 1 ./gauss

	
