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
#include <time.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include "matrix.h"

float* read_matrix(int *dimension) {
    int i, j;
    float *matrix;
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
    matrix = (float*) malloc(sizeof(float) * (*dimension) * (*dimension + 1));
   
    // Recebe a matriz
    for (i = 0; i < *dimension; i++) {
        for (j = 0; j < *dimension; j++) {
            fscanf(matrixInput, "%f", &matrix[i * (*dimension + 1) + j]);
        }
        fscanf(arrayInput, "%f", &matrix[i * (*dimension + 1) + j]);
    }
    // Recebe o vetor no ultima coluna da matriz
   
    fclose(matrixInput);
    fclose(arrayInput);

    return matrix;
}

/*
 *  Usa um rand pra gerar os numeros entre 0 e 50, toda execucao gera o mesmo numero.
 */ 
float* create_matrix(int dimension) {

    int i, j;
    float *matrix;
    
    // For debbuging ...
    //FILE *matrixInput, *arrayInput;

    // matrixInput = fopen("in/matriz_maior.txt", "w");
    // arrayInput= fopen("in/vetor_maior.txt", "w");
    // if (matrixInput == NULL || arrayInput == NULL) {
    //     printf("Erro ao abrir arquivos\n");
    //     exit (1);
    // }

    matrix = (float *) malloc(sizeof(float) * dimension * (dimension + 1));
    for (i = 0; i < dimension; i++) {
        for (j = 0; j < dimension + 1; j++) {
            matrix[i * (dimension + 1) + j] = rand() % 50;
            // if (j != dimension) { 
            //     fprintf(matrixInput, "%.4f ", matrix[i * (dimension + 1) + j]);            
            // }
            // else {
            //     fprintf(arrayInput, "%.4f\n", matrix[i * (dimension + 1) + j]);
            //     fprintf(matrixInput, "\n");            
            // }
        }
    }

    // fclose(matrixInput);
    // fclose(arrayInput);

    return matrix;
}

void destroy_matrix(float *matrix) {
    free(matrix);
}

void print_matrix(int dimension, float *m) {
    int i, j;
    for (i = 0; i < dimension; i++) {
        for (j = 0; j <= dimension; j++) {
            printf("%f ", fabs(m[i * (dimension + 1) + j]));
        }
        printf("\n");
    }
}

void write_result(int dimension, float *solutionArray){
    
    FILE *arq = fopen("out/resultado.txt", "w");
    int i;
    
    for (i = 0; i < dimension; i++){
        fprintf(arq, "%.3f\n", solutionArray[i]);
    }
}

float* create_local_cols(int colsNum, int dimension) {
    float *myCols;
    myCols = (float *) malloc(colsNum * dimension * sizeof(float));
    return myCols;
}

void destroy_local_cols(float *cols) {
    free(cols);
}