//CFLAGS=-g -Wall
CFLAGS= -Wall -O2 

all:	sudoku tags

tests:	matrixtest testexactcov test

sudoku:	sudoku.o matrix.o exactcov.o
	$(CC) $(CFLAGS) -o sudoku sudoku.o matrix.o exactcov.o

matrixtest: matrix.o matrixtest.o
	$(CC) $(CFLAGS) -o matrixtest matrixtest.o matrix.o

test: test.o
	$(CC) $(CFLAGS) -o test test.o

testexactcov: testexactcov.o exactcov.o matrix.o
	$(CC) $(CFLAGS) -o testexactcov testexactcov.o exactcov.o matrix.o

tags: *.h *.c
	ctags *[ch]

install: sudoku
	install --strip sudoku /usr/bin

.PHONY: clean

clean:
	rm -f *.o *~ sudoku test matrixtest testexactcov core* tags
