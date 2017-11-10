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

int main (int argc, char **argv) {
    int i, dimension = 0;
    int npes, myrank;
    float **matrix;
    
    matrix = read_matrix(&dimension);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    for (i = 0; i < dimension; i++) {
        if (matrix[i][i] == 0) swap_line(i, dimension, matrix);
        pivotize(i, dimension, matrix);
        scale(i, dimension, matrix);
    }
    
    //for debugging purposes
    print_matrix(dimension, dimension+1, matrix);

    write_result(dimension, matrix);

    destroy_matrix(dimension, matrix);

    MPI_Finalize();
    
    return 0;
}