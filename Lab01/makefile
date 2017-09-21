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

all: clean run_all
run_all: run_3
#######################################################################
$(OBJ)/n_children_loop.o: $(SRC)/n_children_loop.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/n_children_loop: $(OBJ)/n_children_loop.o
	$(CC) -o $@ $< $(CFLAGS)
run_1: $(BIN)/n_children_loop
	$(BIN)/n_children_loop
#######################################################################
$(OBJ)/n_children_rec.o: $(SRC)/n_children_rec.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/n_children_rec: $(OBJ)/n_children_rec.o
	$(CC) -o $@ $< $(CFLAGS)
run_2: $(BIN)/n_children_rec
	$(BIN)/n_children_rec last
#######################################################################
$(OBJ)/compile.o: $(SRC)/compile.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/compile: $(OBJ)/compile.o
	$(CC) -o $@ $< $(CFLAGS)
run_3: $(BIN)/compile
	$(BIN)/compile n_
#######################################################################
# General requirement for make clean, do not comment
# Prevent operation on any files with name clean
.PHONY: clean
# Clean directory when using command make clean
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
