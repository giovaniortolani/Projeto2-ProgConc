#ifndef MATRIX_H
#define MATRIX_H

float** read_matrix(int *dimension);
void destroy_matrix(int dimension, float **matrix);
void print_matrix(int x, int y, float **m);
void write_result(int dimension, float **matrix);

#endif