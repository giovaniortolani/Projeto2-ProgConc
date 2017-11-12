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
    int dimension = 0, npes, myrank;
    float *matrix, *myCols;

    if (argc == 1) {
        matrix = read_matrix(&dimension);
    } else {
        dimension = atoi(argv[1]);
        matrix = create_matrix(dimension);
    }

    MPI_Init(&argc, &argv);

    // Cada processo recebe sua "parte" do problema
    myCols = initialize(matrix, dimension, &npes, &myrank);

    // Solução Sequencial
    solution_sequential(matrix, dimension);

    //for debugging purposes
    // print_matrix(dimension, matrix);

    write_result(dimension, matrix);

    destroy_matrix(matrix);
    destroy_local_cols(myCols);
    MPI_Finalize();
    
    return 0;
}
