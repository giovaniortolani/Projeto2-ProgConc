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
    Para rodar com uma matriz de dimensao N com M processos.
        $ make run NPES=<M> DIM=<N>
        Ex: $ make run NPES=2 DIM=4
*/

// LEMBRAR QUE A RESPOSTA DEVE ESTAR NA ORDEM ORIGINAL MESMO APOS A TROCA DE LINHA

int main (int argc, char **argv) {
    int dimension = 0, groupSize, count, i;
    int npes, myrank;
    float *matrix, *myCols, *solutionArray;
    MPI_Datatype sendCol, sendColType, recvCol, recvColType;

    double stime, etime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    MPI_Barrier(MPI_COMM_WORLD);
    stime = MPI_Wtime();

    if (myrank == 0) {
        if (argc == 1) {
            matrix = read_matrix(&dimension);
            if (dimension % npes != 0) {
                printf("Dimensão da matriz não divisível pela quantidade de processos.\n");
                destroy_matrix(matrix);
                MPI_Finalize();
                fflush(0);
                exit(1);
            }
        } else {
            dimension = atoi(argv[1]);
            if (dimension % npes != 0) {
                printf("Dimensão da matriz não divisível pela quantidade de processos.\n");
                MPI_Finalize();
                fflush(0);
                exit(1);
            }
            matrix = create_matrix(dimension);
        }
        // For debbuging ...
        // print_matrix(dimension, matrix);
    }
    MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);
    groupSize = dimension / npes;

    if (!myrank) {
        solutionArray = (float *) malloc(dimension * sizeof(float));
        for (i = 0; i < dimension; i++) {
            solutionArray[i] = matrix[dimension + i * (dimension + 1)];
        }
    } 
    else {
        solutionArray = NULL;
    }

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

    // For debbuging ...
    //if (myrank == 0){
        // for (i = 0; i < dimension * (groupSize + 1); i++) {
        //     printf("Rank=%d, %f\n", myrank, myCols[i]);
        // }
        // printf("\n");
    //}

    // Solução Paralela
    solution(myCols, dimension, npes, myrank, solutionArray);

    // Solução Sequencial
    // solution_sequential(matrix, dimension);

    // For debbuging ...
    // MPI_Gather(myCols, count, recvColType, matrix, count, sendColType, 0, MPI_COMM_WORLD);

    if (myrank == 0) {
        // For debugging ...
        // print_matrix(dimension, matrix);
        // for (i = 0; i < dimension; i++) printf("%f \n", solutionArray[i]);
        // printf("\n");

        //write_result(dimension, solutionArray);
        destroy_local_cols(solutionArray);
        destroy_matrix(matrix);
    }
    destroy_local_cols(myCols);

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    etime = MPI_Wtime();

    MPI_Finalize();

    if (myrank == 0){
        printf("NPES = %d, Dim = %d\tTime = %f\n", npes, dimension, etime - stime);
    }
    
    // ...
    // that's all folks!
    return 0;
}
