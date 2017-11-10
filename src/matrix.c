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
#include "matrix.h"

float** read_matrix(int *dimension) {
    int i, j;
    float **matrix;
    FILE *matrixInput, *arrayInput;

    matrixInput = fopen("in/matriz.txt", "r");
    arrayInput = fopen("in/vetor.txt", "r");
    if (matrixInput == NULL || arrayInput == NULL) {
        printf("Erro ao abrir arquivos de Input\n");
        exit (1);
    }

    // Le o arquivo que contem o vetor para descobrir a dimensão da matriz
    float buf;
    while((fscanf(arrayInput, "%f", &buf)) != EOF) {
        (*dimension)++;
    }
    rewind(arrayInput);
    // Aloca dinamicamente matriz que vai conter os dados
    matrix = (float**) malloc(sizeof(float*) * (*dimension));
    for (i = 0; i < *dimension; i++) {
        matrix[i] = (float*) malloc(sizeof(float) * (*dimension + 1));
    }
    // Recebe a matriz
    for (i = 0; i < *dimension; i++) {
        for (j = 0; j < *dimension; j++) {
            fscanf(matrixInput, "%f", &matrix[i][j]);
        }
    }
    // Recebe o vetor no ultima coluna da matriz
    for (i = 0; i < *dimension; i++) {
        fscanf(arrayInput, "%f", &matrix[i][*dimension]);
    }
    fclose(matrixInput);
    fclose(arrayInput);

    return matrix;
}

void destroy_matrix(int dimension, float **matrix) {
    int i;
    
    for (i = 0; i < dimension; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(int x, int y, float **m) {
    int i, j;
    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            printf("%f ", m[i][j]);
        }
        printf("\n");
    }
}

void write_result(int dimension, float **matrix){
    
    FILE *arq = fopen("out/resultado.txt", "w");
    int i;
    
    for (i = 0; i < dimension; i++){
        fprintf(arq, "%f\n", matrix[i][dimension]);
    }
}