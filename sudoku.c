#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "exactcov.h"

void read_input(int[9][9]);
void print_array(int[9][9]);
matrix *buildmatrix(int[9][9]);
int countnonzero(int[9][9]);
void addrow(matrix * pm, int r, int c, int n);
int lastrow;
void decode(matrix * pm);

void nothing(matrix * pm)
{
}

int main(void)
{
	matrix *pm;
	int input[9][9];
	int num;

	read_input(input);

	print_array(input);
	printf("\n");

	pm = buildmatrix(input);

	num = solveexactcov(pm, decode);

	printf("\n%d solution%s\n", num, num == 1 ? "" : "s");

	return 0;
}

void read_input(int input[9][9])
{
	int i, j;
	char line[12];
	char *ret;

	for (i = 0; i < 9; i++) {
		ret = fgets(line, 12, stdin);
		if (!ret) {
			fprintf(stderr,
				"Line %d: Unexpected EOF\n", i + 1);
			exit(1);
		}
		if (strlen(ret) > 10) {
			fprintf(stderr, "Line %d: Too many characters\n",
				i + 1);
			exit(2);
		}
		if (strlen(ret) < 10) {
			fprintf(stderr, "Line %d: Too few characters\n",
				i + 1);
			exit(3);
		}
		for (j = 0; j < 9; j++) {
			if (line[j] <= '9' && line[j] >= '1')
				input[i][j] = line[j] - '0';
			else
				input[i][j] = 0;
		}
	}
}

void print_array(int arr[9][9])
{
	int i, j, n;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			n = arr[i][j];
			if (n)
				printf("%d ", n);
			else
				printf(". ");
		}
		printf("\n");
	}
}

matrix *buildmatrix(int input[9][9])
{
	int r, c, i, n;
	matrix *pm;
	pm = createmat(729 - 8 * countnonzero(input), 324);
	lastrow = 0;
	for (r = 0; r < 9; r++)
		for (c = 0; c < 9; c++) {
			n = input[r][c];
			if (n)
				addrow(pm, r, c, n);
			else
				for (i = 1; i < 10; i++)
					addrow(pm, r, c, i);
		}
	return pm;
}

void addrow(matrix * pm, int r, int c, int n)
{
	int box = (r / 3) * 3 + (c / 3);
	setvalmat(pm, lastrow, r * 9 + c, 1);
	setvalmat(pm, lastrow, 81 + r * 9 + n - 1, 1);
	setvalmat(pm, lastrow, 162 + c * 9 + n - 1, 1);
	setvalmat(pm, lastrow++, 243 + box * 9 + n - 1, 1);
}

int countnonzero(int input[9][9])
{
	int i = 0;
	int r, c;
	for (r = 0; r < 9; r++)
		for (c = 0; c < 9; c++)
			if (input[r][c])
				i++;
	return i;
}

void decode(matrix * pm)
{
	int sol[9][9];
	int r, c;
	int row, col, n;
	for (r = 0; r < 81; r++) {
		c = 0;
		while (!getvalmat(pm, r, c))
			c++;
		row = c / 9;
		col = c - row * 9;
		c = 81;
		while (!getvalmat(pm, r, c))
			c++;
		n = c - 80 - row * 9;
		sol[row][col] = n;
		//printf("(%d,%d) = %d\n", row, col, n);
	}
	printf("\n");
	print_array(sol);
//      printmat(pm);
}
