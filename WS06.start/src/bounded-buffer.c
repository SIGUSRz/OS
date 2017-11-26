#include <header.h>
#define CAPACITY 100

int queue[CAPACITY];
int length;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void *producer_thread(void *);
void *consumer_thread(void *);
void print_queue();

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int NUM_PROD, NUM_CONS;
    NUM_PROD = atoi(argv[1]);
    NUM_CONS = atoi(argv[1]);
    pthread_t *ptid, *ctid;

    ptid = (pthread_t *)calloc(NUM_PROD, sizeof(pthread_t));
    ctid = (pthread_t *)calloc(NUM_CONS, sizeof(pthread_t));

    int i;
    for (i = 0; i < NUM_PROD; i++) {
        if (pthread_create(&(ptid[i]), NULL, &producer_thread, NULL) != 0) {
            perror("pthread_create producer");
            exit(-1);
        }
    }
    for (i = 0; i < NUM_CONS; i++) {
        if (pthread_create(&(ctid[i]), NULL, &consumer_thread, NULL) != 0) {
            perror("pthread_create consumer");
            exit(-1);
        }
    }

    for (i = 0; i < NUM_PROD; i++) {
        if (pthread_join(ptid[i], NULL) != 0) {
            perror("pthread_join producer");
            kill(getpid(), SIGINT);
        }
    }
    for (i = 0; i < NUM_CONS; i++) {
        if (pthread_join(ctid[i], NULL) != 0) {
            perror("pthread_join consumer");
            kill(getpid(), SIGINT);
        }
    }
    return 0;
}

int enqueue(int * queue, int value, pthread_mutex_t *m) {
    if (length == CAPACITY) {
        pthread_cond_wait(&cond_producer, m);
    }
    queue[length] = value;
    length++;
    pthread_cond_signal(&cond_consumer);
    return 0;
}

int dequeue(int *queue, pthread_mutex_t *m) {
    if (length == 0) {
        pthread_cond_wait(&cond_consumer, m);
    }
    length--;
    int value;
    value = queue[length];
    pthread_cond_signal(&cond_producer);
    return value;
}

void *producer_thread(void *pointer) {
    pthread_mutex_lock(&mutex);
    int value;
    value = rand() % 100;
    printf("produce %d ", value);
    print_queue();
    enqueue(queue, value, &mutex);
    pthread_mutex_unlock(&mutex);
    pthread_exit((void *) 0);
}

void *consumer_thread(void *pointer) {
    pthread_mutex_unlock(&mutex);
    int value;
    value = dequeue(queue, &mutex);
    printf("consume %d\n", value);
    print_queue();
    pthread_mutex_unlock(&mutex);
    pthread_exit((void *) 0);
}

void print_queue() {
    int i = 0;
    for (i = 0; i < length; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}