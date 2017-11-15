# Shi Zeng sz1358 Homework 5

## File Description

src:

    Q1:
        conversion_server.c 
        conversion_client.c
        utils.c
        conversion_test.c
    Q2: 
        wave.c

header:

    Q1:
        header.h

## Makefile Description
## Question 1
* make run_server: compile conversion_server.c  to object file conversion_server.o 
then to binary executable conversion_server, and execute it.
* make run_client: compile conversion_client.c to object file conversion_client.o
then to binary executable conversion_client, and execute it.
* ./run.sh: run run.sh script to compile conversion_client.c and concurrently run several clients. 

## Question 2

A deadlock will happen if no protections are made. The first philosopher tries to
wait for the second to finish, the second waits for the third, ..., the last waits
for the first to finish. All philosophers are waiting for others to finish.
* make run_2: compile wave.c to object file wave.o 
then to binary executable wave

* make run_dinner: compile dinner.c  to object file dinner.o 
then to binary executable dinner, and execute it.
First argument is number of philosophers
Second argument is number of round

