#include "exactcov.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct item {
	struct item *l;
	struct item *r;
	struct item *u;
	struct item *d;
	struct item *c;
	int size;		/* for headers only */
	int col;		/* for headers only */
} item;

item hdr;
item *sol[1000];
int ncols;
long nbrsols;

item *pickcol(void)
{
	int size = 10000000;
	item *res = NULL;
	item *pi;
	pi = hdr.r;
	while (pi != &hdr) {
		if (pi->size < size) {
			size = pi->size;
			res = pi;
		}
		pi = pi->r;
	}
	return res;
}

item *getcol(int c)
{
	item *pi = &hdr;
	while (pi->col != c)
		pi = pi->r;
	return pi;
}

item *getcolbot(int c)
{
	item *col, *pi;
	col = getcol(c);
	pi = col;
	while (pi->d)
		pi = pi->d;
	return pi;
}

void remh(item * pi)
{
	(pi->l)->r = pi->r;
	(pi->r)->l = pi->l;
}

void remv(item * pi)
{
	(pi->u)->d = pi->d;
	(pi->d)->u = pi->u;
	(pi->c)->size -= 1;
}

void insh(item * pi)
{
	(pi->l)->r = pi;
	(pi->r)->l = pi;
}

void insv(item * pi)
{
	(pi->u)->d = pi;
	(pi->d)->u = pi;
	(pi->c)->size += 1;
}

void covercol(item * col)
{
	item *p1, *p2;
	remh(col);
	p1 = col->d;
	while (p1 != col) {
		p2 = p1->r;
		while (p2 != p1) {
			remv(p2);
			p2 = p2->r;
		}
		p1 = p1->d;
	}
}

void uncovercol(item * col)
{
	item *p1, *p2;
	p1 = col->u;
	while (p1 != col) {
		p2 = p1->l;
		while (p2 != p1) {
			insv(p2);
			p2 = p2->l;
		}
		p1 = p1->u;
	}
	insh(col);
}

void initgrid(matrix * pm)
{
	int r, c;

	item *pi;
	item *last = &hdr;
	item *first;

	hdr.col = -1;
	ncols = pm->cols;

	/* insert column headers */
	for (c = 0; c < pm->cols; c++) {
		pi = malloc(sizeof(item));
		pi->l = last;
		last->r = pi;
		pi->col = c;
		pi->size = 0;
		last = pi;
	}
	last->r = &hdr;
	hdr.l = last;

	/* now for the data */
	for (r = 0; r < pm->rows; r++) {
		first = NULL;
		last = NULL;
		for (c = 0; c < pm->cols; c++) {
			if (getvalmat(pm, r, c)) {
				pi = malloc(sizeof(item));
				if (!first)
					first = pi;
				pi->l = last;
				if (last)
					last->r = pi;
				pi->u = getcolbot(c);
				(pi->u)->d = pi;
				pi->c = getcol(c);
				(pi->c)->size += 1;
				last = pi;
			}
		}
		if (first) {
			first->l = last;
			last->r = first;
		}
	}
	/* connect the bottom of the rows down to the column headers */
	for (c = 0; c < pm->cols; c++) {
		first = getcol(c);
		last = getcolbot(c);
		first->u = last;
		last->d = first;
	}
}

void addsol(int level, void (*pt2func)(matrix *))
{
	int i, j;
	item *pi;
	matrix *pm = createmat(level, ncols);
	for (i = 0; i < level; i++)
		for (j = 0; j < ncols; j++)
			setvalmat(pm, i, j, 0);
	for (i = 0; i < level; i++) {
		pi = sol[i];
		do {
			setvalmat(pm, i, (pi->c)->col, 1);
			pi = pi->r;
		} while (pi != sol[i]);
	}
	nbrsols++;
	if (!(nbrsols % 1000))
		printf("%ld\n", nbrsols);
	pt2func(pm);
	destroymat(pm);
}

void search(int level, void (*pt2func)(matrix *))
{
	item *col;
	item *p1, *p2;
	if (hdr.r == &hdr)
		addsol(level, pt2func);
	else {
		col = pickcol();
		if (col) {
			covercol(col);
			p1 = col->d;
			while (p1 != col) {
				sol[level] = p1;
				p2 = p1->r;
				while (p2 != p1) {
					covercol(p2->c);
					p2 = p2->r;
				}
				search(level + 1, pt2func);
				p2 = p1->l;
				while (p2 != p1) {
					uncovercol(p2->c);
					p2 = p2->l;
				}
				p1 = p1->d;
			}
			uncovercol(col);
		}
	}
}

int solveexactcov(matrix * pm, void (*pt2func)(matrix *))
{
	nbrsols = 0;
	initgrid(pm);
	search(0, pt2func);
	return nbrsols;
}
