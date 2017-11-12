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

float* initialize(float *matrix, int dimension, int *npes, int *myrank) {
	int groupSize, count;
	float *cols;
    MPI_Datatype sendCol, sendColType, recvCol, recvColType;

    MPI_Comm_size(MPI_COMM_WORLD, npes);
    MPI_Comm_rank(MPI_COMM_WORLD, myrank);

    if (dimension % (*npes) != 0) {
        printf("Dimensão da matriz não divisível pela quantidade de processos.\n");
        fflush(0);
        MPI_Finalize();
        exit(1);
    }
    groupSize = dimension / (*npes);

    // Cria matriz (em forma de vetor) onde um processo recebe suas colunas
    cols = (float *) calloc(groupSize * dimension, sizeof(float));

    // Cria os tipos de dados para obter coluna das matrizes representadas em vetores
    // O stride para receber é diferente para enviar (só p0 envia)
    if (!myrank) {
        MPI_Type_vector(dimension, 1, dimension + 1, MPI_FLOAT, &sendCol);
        MPI_Type_commit(&sendCol);
        MPI_Type_create_resized(sendCol, 0, sizeof(float), &sendColType);
    }
    MPI_Type_vector(dimension, 1, groupSize, MPI_FLOAT, &recvCol);
    MPI_Type_commit(&recvCol);
    MPI_Type_create_resized(recvCol, 0, sizeof(float), &recvColType);

    count = groupSize;
    // MPI_Scatter(matrix, count, sendColType, cols, count, recvColType, 0, MPI_COMM_WORLD);

	return cols;
}

void destroy_local_cols(float *cols) {
    free(cols);
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
