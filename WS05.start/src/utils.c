#include <header.h>

/* Converts from any to EUR */
double convertfrom(int input_currency, double input_amount) {
    if (input_currency == 1)
        return (input_amount / GBP);
    else if (input_currency == 2)
        return (input_amount / USD);
    else if (input_currency == 3)
        return (input_amount / JPY);
    else if (input_currency == 4)
        return (input_amount / CNY);
    else if (input_currency == 0)
        return input_amount;
    else {
        perror("Unknown currency");
        exit(1);
    }
}


/* Converts from EUR to any */
double convertto(int target_currency, double input_amount) {
    if (target_currency == 1)
        return (input_amount * GBP);
    else if (target_currency == 2)
        return (input_amount * USD);
    else if (target_currency == 3)
        return (input_amount * JPY);
    else if (target_currency == 4)
        return (input_amount * CNY);
    else if (target_currency == 0)
        return input_amount;
    else {
        perror("Unknown currency");
        exit(1);
    }
}

/* Determines a currency string identifier, given its integer identifier */
char* determine_currency(int curr_nb) {
    switch(curr_nb) {
        case EUR_CONV: return "EUR";
        case GBP_CONV: return "GBP";
        case USD_CONV: return "USD";
        case JPY_CONV: return "JPY";
        case CNY_CONV: return "CNY";
        default: perror("Unknown currency"); exit(1);
    }
}

int determine_currency_idx(char* input_currency) {
    if (strcmp(input_currency, "GBP") == 0)
        return 1;
    else if (strcmp(input_currency, "USD") == 0)
        return 2;
    else if (strcmp(input_currency, "JPY") == 0)
        return 3;
    else if (strcmp(input_currency, "CNY") == 0)
        return 4;
    else if (strcmp(input_currency, "EUR") == 0)
        return 0;
    else {
        perror("Unknown currency");
        exit(-1);
    }
}


/* Converts from one currency to another.
	input_currency :	currency to convert from
	target_currency :	currency to convert to
	input_amount :		amount to convert */
double convert(int input_currency, int target_currency, double input_amount) {
    return convertto(target_currency, convertfrom(input_currency, input_amount));
}

int enqueue(req_queue *queue, request *value) {
    int index;
    index = (queue->head + queue->length) % LENGTH;
    queue->array[index] = *value;
    queue->length++;
    return 0;
}

int dequeue(req_queue *queue, request *value) {
    *value = queue->array[queue->head];
    queue->head = (queue->head + 1) % LENGTH;
    queue->length--;
    return 0;
}