CFLAGS=-Iinclude -Wall
BIN = bin
INC = include
LIB = lib
OBJ = obj
SRC = src
all: build run

build: clean
	gcc $(CFLAGS) -c -o obj/pending-redefine.o src/pending-redefine.c
	gcc $(CFLAGS) -o bin/pending-redefine obj/pending-redefine.o

run:
	bin/pending-redefine

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
