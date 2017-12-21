#ifndef GAUSS_H
#define GAUSS_H

void solution(float *myCols, int dimension, int npes, int myrank, float *solutionArray, int OPENMP);
void solution_sequential(float *matrix, int dimension);

#endif
