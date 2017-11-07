# C Compiler to use
CC = gcc
# Parameter to pass in compile sentence
CFLAGS = -std=c99 -Wall -I$(INC)
# Define macros, used as variables
BIN = bin
INC = include
LIB = lib
OBJ = obj
SRC = src
OUT = out

all: clean run_all
run_all: run_3
#######################################################################
$(OBJ)/sleep.o: $(SRC)/sleep.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(OBJ)/sleep-test.o: $(SRC)/sleep-test.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/sleep-test: $(OBJ)/sleep-test.o $(OBJ)/sleep.o
	$(CC) -o $@ $(OBJ)/sleep-test.o $(OBJ)/sleep.o $(CFLAGS)
run_1: $(BIN)/sleep-test
	$(BIN)/sleep-test
#######################################################################
$(OBJ)/wave.o: $(SRC)/wave.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/wave: $(OBJ)/wave.o
	$(CC) -o $@ $< $(CFLAGS)
run_2: $(BIN)/wave
	$(BIN)/wave
#######################################################################
$(OBJ)/sync.o: $(SRC)/sync.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/sync: $(OBJ)/sync.o
	$(CC) -o $@ $< $(CFLAGS)
run_3: $(BIN)/sync
	$(BIN)/sync
#######################################################################
# General requirement for make clean, do not comment
# Prevent operation on any files with name clean
.PHONY: clean
# Clean directory when using command make clean
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
