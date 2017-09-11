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

#######################################################################
# Notes, do not uncomment
# Define a rule to compile all .c files into obj/.o files
# This rule is the combination of the above two rules
# The .o files depend on .c files with same name and the dependency list(DEP), the stack.h file
# -c: to generate the object file
# -o $@: put the output of the compilation in the file named on the left side of the ':'
# $<: the first item in the dependency list [src/%.c, include/.h]
# $(OBJ)/%.o: $(SRC)/%.c $(INC)/$(DEP)
# 	$(CC) -c -o $@ $< $(CFLAGS)
#######################################################################
# Make for question 1-3
all: run_all
$(OBJ)/stack_test.o: $(SRC)/stack_test.c $(INC)/stack.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)/stack_array.o: $(SRC)/stack_array.c $(INC)/stack.h
	$(CC) -c -o $@ -D STACK_SIZE=5 $< $(CFLAGS)

$(LIB)/libarray.a: $(OBJ)/stack_array.o
	ar rs $@ $<

$(BIN)/array_test: $(OBJ)/stack_test.o $(LIB)/libarray.a
	$(CC) -o $@ $^ $(CFLAGS)

run_array: $(BIN)/array_test
	echo "Testing Stack"
	$(BIN)/array_test
#######################################################################
# Make for questions 4, 5
$(OBJ)/list_impl.o: $(SRC)/list_impl.c $(INC)/list.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)/stack_list.o: $(SRC)/stack_list.c $(OBJ)/list_impl.o $(INC)/stack.h $(INC)/list.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB)/libstack.a: $(OBJ)/stack_list.o $(OBJ)/list_impl.o
	ar rs $@ $(OBJ)/stack_list.o $(OBJ)/list_impl.o

$(BIN)/list_test: $(OBJ)/stack_test.o $(LIB)/libstack.a $(INC)/stack.h $(INC)/list.h
	$(CC) -o $@ $(OBJ)/stack_test.o $(LIB)/libstack.a $(CFLAGS)

run_list: $(BIN)/list_test
	echo "Testing Linked List"
	$(BIN)/list_test
#######################################################################
# Make for question 6
$(OBJ)/fifo_list.o: $(SRC)/fifo_list.c $(LIB)/libstack.a $(INC)/fifo.h $(INC)/list.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)/fifo_test.o: $(SRC)/fifo_test.c $(INC)/fifo.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB)/libfifo.a: $(OBJ)/fifo_list.o $(OBJ)/list_impl.o
	ar r $@ $^

$(BIN)/fifo_test: $(OBJ)/fifo_test.o $(LIB)/libfifo.a
	$(CC) -o $@ $^ $(CFLAGS)

run_queue: $(BIN)/fifo_test
	echo "Testing Queue"
	$(BIN)/fifo_test

run_all: run_array run_list run_queue
#######################################################################
# General requirement for make clean, do not comment
# Prevent operation on any files with name clean
.PHONY: clean
# Clean directory when using command make clean
clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
