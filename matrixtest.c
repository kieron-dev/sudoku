#include "matrix.h"
#include <stdio.h>

int main(void)
{
	matrix *m;
	int i, j;
	m = createmat(10, 20);
	for (i = 0; i < 10; i++)
		for (j = 0; j < 20; j++)
			setvalmat(m, i, j, 100*i + j);

	for (i = 0; i < 10; i ++)
		for (j = 0; j < 20; j++)
			printf("(%2d,%2d) = %d\n", i, j, getvalmat(m, i, j));
	printmat(m);
	destroymat(m);
	return 0;
}
