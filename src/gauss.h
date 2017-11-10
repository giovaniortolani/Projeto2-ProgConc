#ifndef GAUSS_H
#define GAUSS_H

void swap_line(int line, int dimension, float **matrix);
void pivotize(int line, int dimension, float **matrix);
void scale(int line, int dimension, float **matrix);

#endif