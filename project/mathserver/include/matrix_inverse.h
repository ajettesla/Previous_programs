#ifndef _MATRIX_INVERSE_H
#define _MATRIX_INVERSE_H
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 4096
typedef double matrix[MAX_SIZE][MAX_SIZE];
extern int N;
extern double A[MAX_SIZE][MAX_SIZE];  // Note the size of the array.
extern double I[MAX_SIZE][MAX_SIZE];
extern int maxnum;
extern char* Init;
extern int PRINT;



/* forward declarations */
void find_inverse();
void Init_Matrix(char *buffer);
void Print_Matrix(matrix M, char name[], char *buffer);
void Init_Default();
int Read_Options(int, char**, char *buffer);
void startcalulation(char *buffer, int argc, char** argv);

#endif

