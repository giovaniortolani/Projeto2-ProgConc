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

/* 
    Para rodar com a matriz do professor. 
        $ make run
    Para rodar com uma matriz de dimensao N criada por nos.
        $ make run DIM=<N>
        Ex: $ make run DIM=4
*/

// LEMBRAR QUE A RESPOSTA DEVE ESTAR NA ORDEM ORIGINAL MESMO APOS A TROCA DE LINHA

int main (int argc, char **argv) {
    int i, dimension = 0, groupSize, count;
    int npes, myrank;
    float *matrix, *myCols;

    MPI_Status status;
    MPI_Datatype sendCol, sendColType, recvCol, recvColType;
    
    printf("%d\n", argc);
    if (argc == 1) {
        matrix = read_matrix(&dimension);
    }
    else {
        dimension = atoi(argv[1]);
        matrix = create_matrix(dimension);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (dimension % npes != 0) {
        printf("Dimensão da matriz não divisível pela quantidade de processos.\n");
        fflush(0);
        MPI_Finalize();
        exit(1);
    }
    groupSize = dimension / npes;

    // Cria matriz (em forma de vetor) onde um processo recebe suas colunas
    myCols = create_local_cols(groupSize, dimension);

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
    MPI_Scatter(matrix, count, sendColType, myCols, count, recvColType, 0, MPI_COMM_WORLD);

    if (myrank == 0){
        for (i = 0; i < (groupSize * dimension); i++) {
            printf("%f, ", myCols[i]);
        }
        printf("\n");
    }

    // Debug
    // for (i = 0; i < dimension; i++) {
    //     if (matrix[i * (dimension + 1) + i] == 0) swap_line(i, dimension, matrix);
    //     pivotize(i, dimension, matrix);
    //     scale(i, dimension, matrix);
    // }

    //for debugging purposes
    // print_matrix(dimension, matrix);

    write_result(dimension, matrix);

    destroy_matrix(matrix);
    destroy_local_cols(myCols);
    MPI_Finalize();
    
    return 0;
}
