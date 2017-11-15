#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

/* Identifier for the converter in euros */
#define EUR_CONV 0
/* Identifier for the converter in pounds sterling */
#define GBP_CONV 1
/* Identifier for the converter in US dollars */
#define USD_CONV 2
/* Identifier for the converter in japanese yen */
#define JPY_CONV 3
/* Identifier for the converter in chinese yuan */
#define CNY_CONV 4

/* Conversion rate euro */
#define EUR 1
/* Conversion rate pound sterling */
#define GBP 0.8388
/* Conversion rate US dollar */
#define USD 1.1255
/* Conversion rate yen */
#define JPY 114.096
/* Conversion rate yuan */
#define CNY 7.4986

#define N 5

#define LENGTH 3
typedef struct request {
    int client_id;
    int input_currency;
    float amount;
} request;
typedef struct req_queue {
    struct request array[LENGTH];
    int head;
    int length;
} req_queue;

typedef struct response {
    float result[N];
} response;

/* Converts from a currency to another.
	input_currency :	currency to convert from
	target_currency :	currency to convert to
	input_amount :		amount to convert */
double convert(int input_currency, int target_currency, double input_amount);
int determine_currency_idx(char* input_currency);
char* determine_currency(int curr_nb);

int enqueue(req_queue *queue, request *value);
int dequeue(req_queue *queue, request *value);

