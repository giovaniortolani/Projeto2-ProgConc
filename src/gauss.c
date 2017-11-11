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

void swap_line(int line, int dimension, float *matrix) {

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

void pivotize(int line, int dimension, float *matrix) {
    
    float divisor = matrix[line * (dimension + 1) + line];
    int i;
    
    for (i = 0; i <= dimension; i++) {
		matrix[line * (dimension + 1) + i] /= divisor;
	}
    
}

void scale(int line, int dimension, float *matrix) {
    
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

