# C Compiler to use
CC = gcc
# Parameter to pass in compile sentence
CFLAGS = -Wall -I$(INC) -D_GNU_SOURCE -lrt -lpthread -lm
# Define macros, used as variables
BIN = bin
INC = include
LIB = lib
OBJ = obj
SRC = src
OUT = out

######################################################################
$(OBJ)/upper.o: $(SRC)/upper.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/upper: $(OBJ)/upper.o
	$(CC) -o $@ $< $(CFLAGS)
run_upper: $(BIN)/upper
	$(BIN)/upper upper.txt guess.txt
######################################################################
$(OBJ)/mutex_upper.o: $(SRC)/mutex_upper.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/mutex_upper: $(OBJ)/mutex_upper.o
	$(CC) -o $@ $< $(CFLAGS)
run_mutex: $(BIN)/mutex_upper
	$(BIN)/mutex_upper upper.txt guess.txt taker.txt warden.txt
######################################################################
$(OBJ)/bounded-buffer.o: $(SRC)/bounded-buffer.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/bounded-buffer: $(OBJ)/bounded-buffer.o
	$(CC) -o $@ $< $(CFLAGS)
run_buffer: $(BIN)/bounded-buffer
	$(BIN)/bounded-buffer 120
######################################################################
$(OBJ)/chain.o: $(SRC)/chain.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/chain: $(OBJ)/chain.o
	$(CC) -o $@ $< $(CFLAGS)
run_chain: $(BIN)/chain
	$(BIN)/chain 5
######################################################################
# General requirement for make clean, do not comment
# Prevent operation on any files with name clean
.PHONY: clean
# Clean directory when using command make clean
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/* ${OUT}/*
