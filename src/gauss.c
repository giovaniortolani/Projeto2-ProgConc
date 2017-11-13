/********************************************************
*   Projeto 2 - Implementação Método de Gauss-Jordan    *
*   12 Novembro 2017                                    *
*                                                       *
*   8598861 - Bernardo Simões Lage Gomes Duarte         *
*   8936648 - Giovani Ortolani Barbosa                  *
*   9066491 - Jorge Luiz da Silva Vilaça                *
*   8937308 - Luiz Augusto Vieira Manoel                *
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include "gauss.h"
#include "matrix.h"

void swap_lines(float *myCols, int psize, int oldPivot, int newPivot) {
	int i, row, aux;

	for (i = 0; i < psize; i++) {
		row = i * psize;
		aux = myCols[row + oldPivot];
		myCols[row + oldPivot] = myCols[row + newPivot];
		myCols[row + newPivot] = aux;
	}
}

void pivotize(float *myCols, float pivot, int psize, int line) {
	int i;	
	for (i = 0; i < psize; i++) {
		myCols[psize * line + i] = myCols[psize * line + i] / pivot; 
	}
}

void scale(float *myCols, float *pivotCol, int dimension, int psize, int line) {
	int i, j;
	for (i = 0; i < psize; i++) {
		for (j = 0; j < dimension; j++) {
			if (j != line) myCols[j * psize + i] -= pivotCol[j] * myCols[line * psize + i];
		}
	}
}

int process_of_column(int column, int npes, int dimension) {
	return (column * npes / dimension);
}

int my_column(int column, int dimension, int npes, int myrank) {
	if (process_of_column(column, npes, dimension) == myrank) return 1;
	return 0;
}

void solution(float *myCols, int dimension, int npes, int myrank) {
	int i, k, psize, innerOffset, pivotIdx, root;
	float *pivotCol, pivot;
	pivotCol = (float*) calloc(dimension, sizeof(float));

	for (k = 0; k < dimension; k++) {
		psize = dimension / npes;
		if (my_column(k, dimension, npes, myrank)) {	// Processo corrente tem pivot atual
			//printf("rank: %d\n", myrank);
			pivotIdx = k;
			innerOffset = k % psize;
			// Copia a coluna pivô para um vetor unico;
			for (i = 0; i < dimension; i++) {
				pivotCol[i] = myCols[i * psize + innerOffset];
			}
			pivot = pivotCol[k];

			if (pivotCol[k] == 0) { 				// swap lines
				for (i = 0; i < dimension; i++) {	// procura novo pivot não nulo
					if (pivotCol[i] != 0) {
						pivotIdx = i;
					}
				}
			}
			

		}
		// Bcast do indice do pivô atual (pivotIdx)
		// Se não tiver que trocar, os processo serão capazes de perceber que o pivotIdx == k
		root = process_of_column(k, npes, dimension);
		MPI_Bcast(&pivotIdx, 1, MPI_INT, root, MPI_COMM_WORLD);

		if (pivotIdx != k) {	// Necessidade de trocar o pivot
			swap_lines(myCols, psize, k, pivotIdx);
		}

		// Bcast do do pivô atual para pivoteamento
		// Root não mudou
		MPI_Bcast(&pivot, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

		//printf("myCols[k]: %f, psize: %d, pivot: %f rank: %d\n", myCols[k], psize, pivot, myrank);

		pivotize(myCols, pivot, psize, k);

		// Bcast da coluna que possui o pivô
		// Root ainda não mudou
		 MPI_Bcast(pivotCol, dimension, MPI_FLOAT, root, MPI_COMM_WORLD);
		//printf("root: %d, myCols[k]: %f, psize: %d, pivotCol[k]: %f rank: %d\n", root, myCols[k], psize, pivotCol[k], myrank);
		  scale(myCols, pivotCol, dimension, psize, k);
		//  for (i = 0; i < dimension; i++) printf("%f \n", pivotCol[i]);
		//  printf("\n");

	}
}

void swap_line_sequential(int line, int dimension, float *matrix) {

	int swap = 0;
	int aux;
	int i;

	if (matrix[line * (dimension + 1) + line] == 0){
		//if you jave to swap the lines
		for (i = 0; i < dimension; i++){
			if (matrix[i * (dimension + 1) + line] != 0){
				swap = i;
				break;
			}
			//find a swappable line
		}
				
		//swap them
		for (i = 0; i <= dimension; i++){
			aux = matrix[line * (dimension + 1) + i];
			matrix[line * (dimension + 1) + i] = matrix[swap * (dimension + 1) + i];
			matrix[swap * (dimension + 1) + i] = aux;
		}
		
		
	}
}

void pivotize_sequential(int line, int dimension, float *matrix) {
    
    float divisor = matrix[line * (dimension + 1) + line];
    int i;
    
    for (i = 0; i <= dimension; i++) {
		matrix[line * (dimension + 1) + i] /= divisor;
	}
    
}

void scale_sequential(int line, int dimension, float *matrix) {
    
    int i, j;
    float *vet = (float*) malloc (sizeof(float) * dimension);
    
    for (i = 0; i < dimension; i++){
		vet[i] = matrix[i * (dimension + 1) + line];
	}
    
    for (i = 0; i < dimension; i++){
	
		if (i != line){
			
			for (j = 0; j <= dimension; j++){
			
					matrix[i * (dimension + 1) + j] -= vet[i] * matrix[line * (dimension + 1) + j];
			}
		}
	}

	free(vet);
}

void solution_sequential(float *matrix, int dimension) {
	int i;

	for (i = 0; i < dimension; i++) {
        if (matrix[i * (dimension + 1) + i] == 0) swap_line_sequential(i, dimension, matrix);
        pivotize_sequential(i, dimension, matrix);
        scale_sequential(i, dimension, matrix);
    }
}
