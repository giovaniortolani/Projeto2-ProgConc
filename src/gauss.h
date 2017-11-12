#ifndef GAUSS_H
#define GAUSS_H

float* initialize(float *matrix, int dimension, int *npes, int *myrank);
void destroy_local_cols(float *cols);
void solution_sequential(float *matrix, int dimension);

#endif
