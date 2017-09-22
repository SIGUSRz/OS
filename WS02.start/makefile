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
run_all: run_1 run_2 run_3
#######################################################################
$(OBJ)/trace.o: $(SRC)/trace.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/trace: $(OBJ)/trace.o
	$(CC) -o $@ $< $(CFLAGS)
run_1: $(BIN)/trace
	$(BIN)/trace
#######################################################################
$(OBJ)/decoder.o: $(SRC)/decoder.c
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/decoder: $(OBJ)/decoder.o
	$(CC) -o $@ $< $(CFLAGS)
run_2: $(BIN)/decoder
	$(BIN)/decoder
#######################################################################
$(OBJ)/converters.o: $(SRC)/converters.c $(INC)/converters.h
	$(CC) -c -o $@ $< $(CFLAGS)
$(LIB)/lib.a: $(OBJ)/converters.o
	ar rs $@ $<
$(OBJ)/multi_converter.o: $(SRC)/multi_converter.c $(INC)/converters.h
	$(CC) -c -o $@ $< $(CFLAGS)
$(BIN)/multi_converter: $(OBJ)/multi_converter.o $(LIB)/lib.a $(INC)/converters.h
	$(CC) -o $@ $(OBJ)/multi_converter.o $(LIB)/lib.a $(CFLAGS)
run_3: $(BIN)/multi_converter
	$(BIN)/multi_converter CNY 100.000
#######################################################################
# General requirement for make clean, do not comment
# Prevent operation on any files with name clean
.PHONY: clean
# Clean directory when using command make clean
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
