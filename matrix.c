#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

matrix *createmat(int rows, int cols)
{
	matrix * pm = malloc(sizeof(matrix));
	if (!pm) {
		fprintf(stderr, "can't alloc memory");
		exit(1);
	}
	pm->data = malloc(sizeof(int) * rows * cols);
	if (!pm->data) {
		fprintf(stderr, "can't alloc memory %dx%d\n", rows, cols);
		exit(2);
	}
	pm->rows = rows;
	pm->cols = cols;
	memset(pm->data, 0, sizeof(int) * rows * cols);
	return pm;
}

void destroymat(matrix *p) 
{
	free(p->data);
	free(p);
}

void setvalmat(matrix *p, int row, int col, int val)
{
	if (row >= p->rows || col >= p->cols || row < 0 || col < 0) {
		fprintf(stderr, "invalid row/col args (%d, %d)\n", row, col);
		exit(3);
	}
	int *dt = p->data;
	dt += row * p->cols + col;
	*dt = val;
}

int getvalmat(matrix *p, int row, int col)
{
	if (row >= p->rows || col >= p->cols || row < 0 || col < 0) {
		fprintf(stderr, "invalid row/col args (%d, %d)\n", row, col);
		exit(4);
	}
	int *dt = p->data;
	dt += row * p->cols + col;
	return *dt;
}

void printmat(matrix *p)
{
	int i, j, n;
	for (i = 0; i < p->rows; i++) {
		for (j = 0; j < p->cols; j++) {
			n = getvalmat(p, i, j);
			printf("%d ", n);
		}
		printf("\n");
	}
	printf("\n");
}
	

