#include <header.h>

char *client_memory, *server_memory;
response *res;
req_queue *queue;
char *sem_queue_name, *sem_req_name, *sem_serverRecv_name, *sem_clientRecv_name;
sem_t *sem_queue, *sem_req, *sem_serverRecv, *sem_clientRecv;

void newClientResultMemory(int client_id);
void quoteServerQueueMemory(int server_id);
void newClientSemaphore(int sever_id, int client_id);
void clean(int signal);

int main(int argc, char *argv[]) {
    int server_id, client_id;
    int input_currency;
    float amount;

    server_id = atoi(argv[1]);
    client_id = atoi(argv[2]);
    input_currency = determine_currency_idx(argv[3]);
    amount = atof(argv[4]);

    printf("Start Request\n");
    newClientResultMemory(client_id);
    quoteServerQueueMemory(server_id);
    newClientSemaphore(server_id, client_id);

    struct sigaction action;
    action.sa_handler = &clean;
    sigaction(SIGINT, &action, NULL);

    sem_wait(sem_req);
    request req;
    req.client_id = client_id;
    req.input_currency = input_currency;
    req.amount = amount;
    sem_wait(sem_queue);
    enqueue(queue, &req);
    sem_post(sem_queue);
    sem_post(sem_serverRecv);

    sem_wait(sem_clientRecv);
    int i;
    for (i = 0; i < N; i++) {
        printf("%.2f %s -> %.2f %s\n", amount, argv[3], res->result[i], determine_currency(i));
    }

    clean(SIGINT);

    return 0;
}

void newClientResultMemory(int client_id) {
    int client_fd;
    asprintf(&client_memory, "/client_%d_shm:0", client_id);
    if ((client_fd = shm_open(client_memory, O_RDWR | O_CREAT, 0600)) == -1) {
        perror("client_memory shm_open");
        exit(-1);
    }
    if ((ftruncate(client_fd, sizeof(response))) == -1) {
        perror("client_memory ftruncate");
        exit(-1);
    }
    res = (response *)mmap(NULL, sizeof(response), PROT_READ | PROT_WRITE, MAP_SHARED, client_fd, 0);
}

void quoteServerQueueMemory(int server_id) {
    int server_fd;
    asprintf(&server_memory, "/server_%d_shm:0", server_id);
    if ((server_fd = shm_open(server_memory, O_RDWR, 0600)) == -1) {
        perror("server_queue shm_open");
        exit(-1);
    }
    queue = (req_queue *)mmap(NULL, sizeof(req_queue), PROT_READ | PROT_WRITE, MAP_SHARED, server_fd, 0);
}

void newClientSemaphore(int server_id, int client_id) {
    asprintf(&sem_queue_name, "/queue_%d_sem", server_id);
    if ((sem_queue = sem_open(sem_queue_name, O_RDWR)) == SEM_FAILED) {
        perror("sem_queue sem_open failed");
        exit(-1);
    }

    asprintf(&sem_req_name, "/req_%d_sem", server_id);
    if ((sem_req = sem_open(sem_req_name, O_RDWR)) == SEM_FAILED) {
        perror("sem_req sem_open failed");
        exit(-1);
    }

    asprintf(&sem_serverRecv_name, "/serverRecv_%d_sem", server_id);
    if ((sem_serverRecv = sem_open(sem_serverRecv_name, O_RDWR)) == SEM_FAILED) {
        perror("sem_serverRecv sem_open failed");
        exit(-1);
    }

    asprintf(&sem_clientRecv_name, "/clientRecv_%d_sem", client_id);
    if ((sem_clientRecv = sem_open(sem_clientRecv_name, O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED) {
        perror("sem_serverRecv sem_open failed");
        exit(-1);
    }
}

void clean(int signal) {
    sem_close(sem_queue);
    sem_close(sem_req);
    sem_close(sem_serverRecv);
    sem_close(sem_clientRecv);

    sem_unlink(sem_clientRecv_name);
    free(sem_queue_name);
    free(sem_req_name);
    free(sem_serverRecv_name);

    munmap(queue, sizeof(req_queue));
    munmap(res, sizeof(float));

    shm_unlink(client_memory);

    printf("Done Clean\n");

    exit(0);
}