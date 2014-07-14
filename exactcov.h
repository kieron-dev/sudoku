#ifndef _EXACTCOV_H_
#define _EXACTCOV_H_

#include "matrix.h"
typedef struct sollist {
	matrix *pm;
	struct sollist *next;
} sollist;

int solveexactcov(matrix * init, void (*pt2func)(matrix *));

#endif
