#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct matrix {
	int rows;
	int cols;
	int *data;
} matrix;

matrix *createmat(int rows, int cols);

void destroymat(matrix *p);

void setvalmat(matrix *p, int row, int col, int val);

int getvalmat(matrix *p, int row, int col);

void printmat(matrix *p);

#endif
