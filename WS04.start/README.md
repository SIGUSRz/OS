# Shi Zeng sz1358 Homework 4

## File Description

src:

    Q1:
        sleep.c 
        sleep_sync.c
    Q2: 
        wave.c
    Q3:
        double_sync.c
        single_sync.c

header:

    Q1:
        head.h

## Makefile Description
## Question 1
* make run_1: compile sleep_test.c to object file sleep_test.o 
then to binary executable sleep_test

## Question 2
* make run_2: compile wave.c to object file wave.o 
then to binary executable wave

## Question 3.1
Minimum Number of Signal Emission: 4

Implemented in double_sync.c using SIGUSR1 and SIGUSR2

* make run_3: compile double_sync.c to object file double_sync.o 
then to binary executable double_sync

## Question 3.2
It's possible.

Implemented in single_sync.c using only SIGUSR1

* make run_4: compile single_sync.c to object file single_sync.o 
then to binary executable single_sync

