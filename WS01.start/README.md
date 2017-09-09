# Shi Zeng sz1358 Homework 1

## Comments:
* This uploaded file is cleaned for compression, to check all files listed in the "File List" section, make the makefile according to the following instruction:
    - There are three sections in the makefile, comment the other two when running a section.

* Extra code for Q7 are labelled with comment "Extra for Q7" in list.h and list_impl.c

* Q3 printing STACK_SIZE is implemented by adding an extra function declared in stack.h called capacity()

## File List:
* /include/
    - fifo.h: head file for questions 6
    - list.h: head file for questions 4, 5, 6
    - stack.h: head file for questions 1, 2, 3

* /lib/
    - libarray.a: library archive for questions 1, 2, 3
    - libfifo.a: library archive for question 6
    - libstack.a: library archive for questions 4, 5

* src/
    - stack_array.c: external function code to implement stack.h for questions 1, 2, 3
    - stack_test.c: main function code for questions 1, 2, 3, 5
    - fifo_list.c: external function code to implement fifo.h for question 6
    - fifo_test.c: main function code for question 6
    - list_impl.c: external function code to implement list.h for questions 4, 5, 6
    - stack_list.c: external function code to implement stack.h using list.h functions for question 4, 5, 6

## Compile Rules:
There are three sections in the makefile, comment the other two when running a section.

* First section makes for questions 1, 2, 3
    1. compile stack_array.c to stack_array.o then archive to libarray.a, with -D setting macro STACK_SIZE
    2. compile stack_test.c to stack_test.o
    3. compile stack_test.o with libarray.a to binary executable stack_test

* Second section makes for questions 4, 5
    1. compile list_impl.c to list_impl.o
    2. compile stack_list.c to stack_list.o using stack.h, list.h and list_impl.o, then archive to libstack.a
    2. compile stack_test.c to stack_test.o using libstack.a, list.h and stack.h
    3. compile stack_test.o with libstack.a to binary executable list_test

* Third section makes for question 6.
    1. compile fifo_list.c to fifo_list.o with libstack.a then archive to libfifo.a
    2. compile fifo_test.c to fifo_test.o
    3. compile stack_test.o with libfifo.a to binary executable fifo_test

## Question 7:
Add a counter into the list_type structure so that to get size just use a function to return list.size

I add some extra codes so that it's possible to determine the size of linked list in O(1)

All extra code is labelled with "Extra for Q7" comment.
