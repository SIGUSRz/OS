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
$(OBJ)/utils.o: $(SRC)/utils.c $(INC)/header.h
	$(CC) -o $@ -c $< $(CFLAGS)
$(OBJ)/conversion_client.o: $(SRC)/conversion_client.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(OBJ)/conversion_server.o: $(SRC)/conversion_server.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/conversion_client: $(OBJ)/conversion_client.o $(OBJ)/utils.o
	$(CC) -o $@ $^ $(CFLAGS)
$(BIN)/conversion_server: $(OBJ)/conversion_server.o $(OBJ)/utils.o
	$(CC) -o $@ $^ $(CFLAGS)
run_client: $(BIN)/conversion_client
run_server: $(BIN)/conversion_server
	$(BIN)/conversion_server 404
######################################################################
$(OBJ)/conversion_test.o: $(SRC)/conversion_test.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/conversion_test: $(OBJ)/conversion_test.o
	$(CC) -o $@ $^ $(CFLAGS)
run: $(BIN)/conversion_test
	$(BIN)/conversion_test 5
######################################################################
$(OBJ)/dinner.o: $(SRC)/dinner.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(BIN)/dinner: $(OBJ)/dinner.o
	$(CC) -o $@ $^ $(CFLAGS)
run_dinner: $(BIN)/dinner
	$(BIN)/dinner 5 2
######################################################################
# General requirement for make clean, do not comment
# Prevent operation on any files with name clean
.PHONY: clean
# Clean directory when using command make clean
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
