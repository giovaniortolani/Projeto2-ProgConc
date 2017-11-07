/********************************************************
*   Projeto 2 - Implementação Método de Gauss-Jordan    *
*   12 Novembro 2017                                    *
*                                                       *
*   8598861 - Bernardo Simões Lage Gomes Duarte         *
*   8936648 - Giovani Ortolani Barbosa                  *
*   9066491 - Jorge Luiz da Silva Vilaça                *
*   8937308 - Luiz Augusto Vieira Manoel                *
********************************************************/
// #include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


void printMatrix(int x, int y, int **m);

int main (int argc, char **argv) {
    int i, j, dimension = 0;
    int **matrix;
    FILE *matrixInput, *arrayInput;

    matrixInput = fopen("in/matriz.txt", "r");
    arrayInput = fopen("in/vetor.txt", "r");
	if (matrixInput == NULL || arrayInput == NULL) {
        printf("Erro ao abrir arquivos de Input\n");
        return 0;
    }

    // Le o arquivo que contem o vetor para descobrir a dimensão da matriz
    int buf;
    while((fscanf(arrayInput, "%d", &buf)) != EOF) {
        dimension++;
    }
    rewind(arrayInput);
    // Aloca dinamicamente matriz que vai conter os dados
    matrix = (int**) malloc(sizeof(int*) * (dimension));
    for (i = 0; i < dimension; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * (dimension + 1));
    }
    // Recebe a matriz
    for (i = 0; i < dimension; i++) {
        for (j = 0; j < dimension; j++) {
            fscanf(matrixInput, "%d", &matrix[i][j]);
        }
    }
    // Recebe o vetor no ultima coluna da matriz
    for (i = 0; i < dimension; i++) {
        fscanf(arrayInput, "%d", &matrix[i][dimension]);
    }
	fclose(matrixInput);
    fclose(arrayInput);
    
    printMatrix(dimension, dimension+1, matrix);

    return 1;
}

void printMatrix(int x, int y, int **m); {
    int i, j;
    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}