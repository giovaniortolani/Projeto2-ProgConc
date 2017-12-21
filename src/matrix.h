#ifndef MATRIX_H
#define MATRIX_H

float* read_matrix(int *dimension);
float* create_matrix(int dimension);
void destroy_matrix(float *matrix);
void print_matrix(int dimension, float *m);
void write_result(int dimension, float *solutionArray);

float* create_local_cols(int colsNum, int dimension);
void destroy_local_cols(float *cols);

#endif
