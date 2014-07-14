#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int create_room(int **);

int main(void)
{
	int i, n;
	int *list;

	n = create_room(&list);

	for (i = 0; i < n; i++)
		printf("list[%d] = %d\n", i, list[i]);

	if (n)
		free(list);
	return 0;
}

int create_room(int **p)
{
	int i, size;
	srand(time(0));
	size = rand() % 5;
	if (size)
		*p = malloc(sizeof(int) * size);
	for (i = 0; i < size; i++)
		(*p)[i] = i;
	return size;
}
