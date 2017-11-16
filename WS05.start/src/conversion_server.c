#include <header.h>

char *server_memory; // Shared File Name of Server Request Queue
req_queue *queue; // Shared Server Queue
char *client_memory[N]; // Shared File Name of Client Result Array
float *res[N]; // Shared Client Memory

// File Names
char *sem_queue_name, *sem_req_name, *sem_serverRecv_name, *sem_clientRecv_name[N];
sem_t *sem_queue; // Semaphore to protect queue action
sem_t *sem_req; // Semaphore to protect pending client request when request queue is full
sem_t *sem_serverRecv; // Semaphore for client to pend and wake server parent process
sem_t *sem_clientRecv[N]; // Semaphore to notify client to read results

char *child_memory[N]; // Shared File Name of Child Process Work Queue
req_queue *cqueue[N]; // Shared Child Work Queue

// File Names
char *sem_cqueue_name[N], *sem_creq_name[N], *sem_childRecv_name[N];
sem_t *sem_cqueue[N]; //Semaphore to protect queue action
sem_t *sem_creq[N]; // Semaphore to protect pending client request when work queue is full
sem_t *sem_childRecv[N]; // Semaphore for server parent to pend and wake server child process

void newServerQueueMemory(int server_id);
void quoteClientResultMemory(int client_id, int i);
void quoteClientSemaphore(int client_id, int i);
void newServerSemaphore(int server_id);
void newChildren(int server_id);
void clean(int signal);
void clean_client_semaphore();
void clean_client_memory();
void clean_children();

int main(int argc, char *argv[]) {
    int server_id;
    server_id = atoi(argv[1]);

    newServerQueueMemory(server_id);
    newServerSemaphore(server_id);
    newChildren(server_id);

    printf("Server Initialized\n");

    struct sigaction action;
    action.sa_handler = &clean;
//    int sig;
//    for (sig = 0; sig < NSIG; sig++) {
//        sigaction(sig, &action, NULL);
//    }
    sigaction(SIGINT, &action, NULL);

    pid_t pid[N];
    int i, j;
    for (i = 0; i < N && (pid[i] = fork()) != 0; i++);

    request req;
    if (i == N) {
        while(1) {
            printf("Waiting For Request\n");
            sem_wait(sem_serverRecv);
            sem_wait(sem_queue);
            dequeue(queue, &req);
            sem_post(sem_queue);
            sem_post(sem_req); // Release to allow more request pending
            printf("Received Request from %d\n", req.client_id);

            for (j = 0; j < N && (pid[j] = fork()) != 0; j++);

            sem_wait(sem_creq[j - 1]);
            sem_wait(sem_cqueue[j - 1]);
            enqueue(cqueue[j - 1], &req);
            sem_post(sem_cqueue[j - 1]);
            sem_post(sem_childRecv[j - 1]);
            if (j != N) {
                exit(0);
            }
        }
    } else {
        while (1) {
            sem_wait(sem_childRecv[i]);
            sem_wait(sem_cqueue[i]);
            dequeue(cqueue[i], &req);
            sem_post(sem_cqueue[i]);
            sem_post(sem_creq[i]);

            quoteClientResultMemory(req.client_id, i);
            quoteClientSemaphore(req.client_id, i);
            *res[i] = convert(req.input_currency, i, req.amount);
            printf("%.2f\n", *res[i]);
            sem_post(sem_clientRecv[i]);
            clean_client_memory(i);
            clean_client_semaphore(i);
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

void newChildren(int server_id) {
    int i;
    for (i = 0; i < N; i++) {
        asprintf(&child_memory[i], "/child_%d_shm:%d", server_id, i);
        int child_fd;
        if ((child_fd = shm_open(child_memory[i], O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
            perror("child shm_open failed");
            exit(-1);
        }
        if (ftruncate(child_fd, sizeof(req_queue)) == -1) {
            perror("child ftruncate failed");
            exit(-1);
        }
        cqueue[i] = (req_queue *)mmap(NULL, sizeof(req_queue),
                                      PROT_READ | PROT_WRITE, MAP_SHARED, child_fd, 0);

        asprintf(&sem_cqueue_name[i], "/cqueue_%d_sem:%d", server_id, i);
        if ((sem_cqueue[i] = sem_open(sem_cqueue_name[i],
                                      O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED) {
            perror("child sem_cqueue sem_open failed");
            exit(-1);
        }

        asprintf(&sem_creq_name[i], "/creq_%d_sem:%d", server_id, i);
        if ((sem_creq[i] = sem_open(sem_creq_name[i],
                                      O_CREAT | O_EXCL | O_RDWR, 0666, N)) == SEM_FAILED) {
            perror("child sem_creq sem_open failed");
            exit(-1);
        }

        asprintf(&sem_childRecv_name[i], "/childRecv_%d_sem:%d", server_id, i);
        if ((sem_childRecv[i] = sem_open(sem_childRecv_name[i],
                                    O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
            perror("child sem_childRecv sem_open failed");
            exit(-1);
        }
    }
}

void quoteClientResultMemory(int client_id, int i) {
    int client_fd;
    asprintf(&client_memory[i], "/client_%d_shm:%d", client_id, i);
    if ((client_fd = shm_open(client_memory[i], O_RDWR, 0600)) == -1) {
        perror("client_memory shm_open failed");
        exit(-1);
    }
    res[i] = (float *)mmap(NULL, sizeof(float), PROT_READ | PROT_WRITE, MAP_SHARED, client_fd, 0);
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

void quoteClientSemaphore(int client_id, int i) {
    asprintf(&sem_clientRecv_name[i], "/clientRecv_%d_sem:%d", client_id, i);
    if ((sem_clientRecv[i] = sem_open(sem_clientRecv_name[i], O_RDWR)) == SEM_FAILED) {
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

    int i;
    for (i = 0; i < N; i++) {
        if (client_memory[i]) {
            clean_client_memory(i);
        }

        if (sem_clientRecv_name[i]) {
            clean_client_semaphore(i);
        }
    }

    clean_children();

    printf("Process Done Clean.\n");

    exit(0);
}

void clean_client_semaphore(int i) {
    sem_close(sem_clientRecv[i]);
    sem_unlink(sem_clientRecv_name[i]);
    free(sem_clientRecv_name[i]);
    sem_clientRecv_name[i] = NULL; // indicate the end of sharing
}

void clean_client_memory(int i) {
    munmap(res[i], sizeof(float));
    shm_unlink(client_memory[i]);
    free(client_memory[i]);
    client_memory[i] = NULL;
}

void clean_children() {
    int i;
    for (i = 0; i < N; i++) {
        if (cqueue[i]) {
            munmap(cqueue[i], sizeof(req_queue));
        }
        if (child_memory[i]) {
            shm_unlink(child_memory[i]);
        }

        if (sem_creq[i]) {
            sem_close(sem_creq[i]);
        }
        if (sem_creq_name[i]) {
            sem_unlink(sem_creq_name[i]);
            free(sem_creq_name[i]);
        }
        if (sem_cqueue[i]) {
            sem_close(sem_cqueue[i]);
        }
        if (sem_cqueue_name[i]) {
            sem_unlink(sem_cqueue_name[i]);
            free(sem_cqueue_name[i]);
        }
        if (sem_childRecv[i]) {
            sem_close(sem_childRecv[i]);
        }
        if (sem_childRecv_name[i]) {
            sem_unlink(sem_childRecv_name[i]);
            free(sem_childRecv_name[i]);
        }
    }
}