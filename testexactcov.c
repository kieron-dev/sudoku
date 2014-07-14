#include "exactcov.h"
#include "matrix.h"

int main(void)
{
	int r, c;
	matrix *pm = createmat(6, 7);
	sollist *sol;

	int data[6][7] = {
		{ 1, 0, 0, 1, 0, 0, 1}, 
		{ 1, 0, 0, 1, 0, 0, 0}, 
		{ 0, 0, 0, 1, 1, 0, 1},
		{ 0, 0, 1, 0, 1, 1, 0}, 
		{ 0, 1, 1, 0, 0, 1, 1}, 
		{ 0, 1, 0, 0, 0, 0, 1}
	};
	for (r = 0; r < 6; r++)
		for (c = 0; c < 7; c++)
			setvalmat(pm, r, c, data[r][c]);
	sol = solveexactcov(pm);
	while (sol) {
		printmat(sol->pm);
		sol = sol->next;
	}
	return 0;
}
