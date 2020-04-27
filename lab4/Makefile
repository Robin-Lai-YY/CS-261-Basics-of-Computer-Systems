all: biginttest

CC=gcc
CLIBS=
CFLAGS=-g -Wall -pedantic -std=c99
LDFLAGS=-g -Wall -pedantic -std=c99

biginttest: biginttest.o bigint.o

biginttest.o: biginttest.c bigint.h
bigint.o: bigint.c bigint.h

clean:
	-rm -rf *.o biginttest
tidy: clean
	-rm -rf *~
