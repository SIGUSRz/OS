#include <header.h>

char *server_memory, *client_memory;
req_queue *queue;
char *sem_queue_name, *sem_req_name, *sem_serverRecv_name, *sem_clientRecv_name;
sem_t *sem_queue, *sem_req, *sem_serverRecv, *sem_clientRecv;
request req;
response *res;

void newServerQueueMemory(int server_id);
void quoteClientResultMemory(int client_id);
void quoteClientSemaphore(int client_id);
void newServerSemaphore(int server_id);
void clean(int signal);
void clean_client_semaphore();
void clean_client_memory();

int main(int argc, char *argv[]) {
    int server_id;
    server_id = atoi(argv[1]);

    newServerQueueMemory(server_id);
    newServerSemaphore(server_id);

    printf("Shared Request Queue Created\n");
    printf("Shared Semaphore Created\n");

    struct sigaction action;
    action.sa_handler = &clean;
//    int sig;
//    for (sig = 0; sig < NSIG; sig++) {
//        sigaction(sig, &action, NULL);
//    }
    sigaction(SIGINT, &action, NULL);

    while(1) {
        printf("Waiting For Request\n");
        sem_wait(sem_serverRecv);
        sem_wait(sem_queue);
        dequeue(queue, &req);
        sem_post(sem_queue);
        sem_post(sem_req); // Release to allow more request pending
        printf("Received Request from %d\n", req.client_id);
        quoteClientResultMemory(req.client_id);
        printf("Shared Result Created\n");

        pid_t pid[N];
        int i;
        for (i = 0; i < N && (pid[i] = fork()) != 0; i++);

        if (i == N) {
            for (i = 0; i < N; i++) {
                wait(NULL);
            }
            printf("Calculation Finished\n");
            quoteClientSemaphore(req.client_id);
            sem_post(sem_clientRecv);
            clean_client_memory();
            clean_client_semaphore();
        } else {
            res->result[i] = convert(req.input_currency, i, req.amount);
            printf("%.2f\n", res->result[i]);
            exit(0);
        }
    }
}

void newServerQueueMemory(int server_id) {
    int server_fd;
    asprintf(&server_memory, "/server_%d_shm:0", server_id);
    if((server_fd = shm_open(server_memory, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
        perror("server_memory shm_open failed");
        exit(-1);
    }
    if (ftruncate(server_fd, sizeof(req_queue)) == -1) {
        perror("server_memory ftruncate failed");
        exit(-1);
    }
    queue = (req_queue *)mmap(NULL, sizeof(req_queue),
                              PROT_READ | PROT_WRITE, MAP_SHARED, server_fd, 0);
}

void quoteClientResultMemory(int client_id) {
    int client_fd;
    asprintf(&client_memory, "/client_%d_shm:0", client_id);
    if ((client_fd = shm_open(client_memory, O_RDWR, 0600)) == -1) {
        perror("client_memory shm_open failed");
        exit(-1);
    }
    res = (response *)mmap(NULL, sizeof(response), PROT_READ | PROT_WRITE, MAP_SHARED, client_fd, 0);
}

void newServerSemaphore(int server_id) {
    asprintf(&sem_queue_name, "/queue_%d_sem", server_id);
    if ((sem_queue = sem_open(sem_queue_name, O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED) {
        perror("sem_queue sem_open failed");
        exit(-1);
    }

    asprintf(&sem_req_name, "/req_%d_sem", server_id);
    if ((sem_req = sem_open(sem_req_name, O_CREAT | O_EXCL | O_RDWR, 0666, LENGTH)) == SEM_FAILED) {
        perror("sem_req sem_open failed");
        exit(-1);
    }

    asprintf(&sem_serverRecv_name, "/serverRecv_%d_sem", server_id);
    if ((sem_serverRecv = sem_open(sem_serverRecv_name,
                                      O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        perror("sem_serverRecv sem_open failed");
        exit(-1);
    }
}

void quoteClientSemaphore(int client_id) {
    asprintf(&sem_clientRecv_name, "/clientRecv_%d_sem", client_id);
    if ((sem_clientRecv = sem_open(sem_clientRecv_name, O_RDWR)) == SEM_FAILED) {
        perror("sem_clientRecv failed");
        exit(-1);
    }
}

void clean(int signal) {
    sem_close(sem_queue);
    sem_unlink(sem_queue_name);
    free(sem_queue_name);

    sem_close(sem_req);
    sem_unlink(sem_req_name);
    free(sem_req_name);

    sem_close(sem_serverRecv);
    sem_unlink(sem_serverRecv_name);
    free(sem_serverRecv_name);

    munmap(queue, sizeof(req_queue));
    shm_unlink(server_memory);
    free(server_memory);
    server_memory = NULL;

    if (client_memory) {
        clean_client_memory();
    }

    if (sem_clientRecv_name) {
        clean_client_semaphore();
    }

    printf("Clean Finished. Exit.\n");

    exit(0);
}

void clean_client_semaphore() {
    sem_close(sem_clientRecv);
    sem_unlink(sem_clientRecv_name);
    free(sem_clientRecv_name);
    sem_clientRecv_name = NULL; // indicate the end of sharing
}

void clean_client_memory() {
    munmap(res, sizeof(response));
    shm_unlink(client_memory);
    free(client_memory);
    client_memory = NULL;
}