#include <os-scheduling.h>


void print_queues(task tasks[], sched_data* schedData) {
    int i, j, taskIndex = 0;
    printf("Nb of queues %d\n", schedData->nbOfQueues);
    for (i = 0; i < schedData->nbOfQueues; i++) {
        j = 0;
        printf("Q%d => ", i);
        while (j < MAX_NB_OF_TASKS) {
            taskIndex = schedData->queues[i][j];
            if (taskIndex == -1) {
                j = MAX_NB_OF_TASKS;
            } else {
                printf("%s ", tasks[taskIndex].name);
                j++;
            }
        }
        printf("\n");
    }
}


int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {

    int i, j;

    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }

    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].waitTime = 0;
            schedData->queues[0][j] = i;
            j++;
        }
        if (tasks[i].state == READY) {
            tasks[i].waitTime++;
        }
    }
    print_queues(tasks, schedData);

    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                /* Reelect this task */
                tasks[i].executionTime++;
                return i;
            }
        }
    }

    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1) {
        tasks[i].executionTime++;
        tasks[i].state = RUNNING;
        return i;
    }

    // No task could be elected
    return -1;
}


int SJF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, minCompTime;

    // N.B: SJF does not require any queue!

    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].waitTime = 0;
        }
        if (tasks[i].state == READY) {
            tasks[i].waitTime++;
        }
    }

    // Is there a task running? Has that task finished its computations?
    //   If so, put it in terminated state
    //   If not, continue this task
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                break;
            } else {
                /* Reelect this task */
                tasks[i].executionTime++;
                return i;
            }
        }
    }

    //Otherwise, find the task in READY state that has the shortest computation time
    j = -1;
    minCompTime = 0;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            if ((j == -1) || (minCompTime > tasks[i].computationTime)) {
                j = i;
                minCompTime = tasks[i].computationTime;
            }
        }
    }
    if (j != -1) {
        tasks[j].executionTime++;
        tasks[j].state = RUNNING;
        return j;
    }

    return -1;
}


int SRTF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, minRemainTime;

    // N.B: SJF does not require any queue!

    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].waitTime = 0;
        }
        if (tasks[i].state == READY) {
            tasks[i].waitTime++;
        }
    }

    // Is there a task running? Has that task finished its computations?
    //   If so, put it in terminated state
    //   If not, continue this task
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                break;
            } else {
                /* Reelect this task */
                // tasks[i].executionTime++;
                // return i;
                tasks[i].state = READY;
                break;
            }
        }
    }

    //Otherwise, find the task in READY state that has the shortest computation time
    j = -1;
    minRemainTime = 0;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            if ((j == -1) || (minRemainTime > tasks[i].computationTime - tasks[i].executionTime)) {
                j = i;
                minRemainTime = tasks[i].computationTime - tasks[i].executionTime;
            }
        }
    }
    if (j != -1) {
        tasks[j].executionTime++;
        tasks[j].state = RUNNING;
        return j;
    }
    return -1;
}


int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum) {

    int i, j;
    int num_task = 0;

    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }

    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].waitTime = 0;
            tasks[i].quanTime = 0;
            schedData->queues[0][j] = i;
            j++;
        }
        if (tasks[i].state == READY) {
            tasks[i].waitTime++;
        }
    }
    num_task = j;
    print_queues(tasks, schedData);

    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = schedData->queues[0][0];
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[0][j] = schedData->queues[0][j+1];
                }
            } else {
                if (tasks[i].quanTime % quantum == 0) {
                    tasks[i].state = READY;
                    tasks[i].quanTime = 0;
                    int temp = schedData->queues[0][0];
                    for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                    }
                    schedData->queues[0][num_task - 1] = temp;
                } else {
                    tasks[i].executionTime++;
                    tasks[i].quanTime++;
                    return i;
                }
            }
        }
    }

    // Otherwise, elect the first task in the queue
    i = schedData->queues[0][0];
    if (i != -1) {
        tasks[i].executionTime++;
        tasks[i].quanTime++;
        tasks[i].state = RUNNING;
        return i;
    }

    // No task could be elected
    return -1;
}


int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum) {
    int i, j, k = 0, l;
    int num_task = 0;

    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 3;
        for (k = 0; k < schedData->nbOfQueues; k++) {
            for (i = 0; i < MAX_NB_OF_TASKS; i++) {
                schedData->queues[k][i] = -1;
            }
        }
    }

    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].waitTime = 0;
            tasks[i].quanTime = 1;
            schedData->queues[0][j] = i;
            j++;
        }
        if (tasks[i].state == READY) {
            tasks[i].waitTime++;
        }
    }
    num_task = j;
    print_queues(tasks, schedData);

    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    for (k = 0; k < schedData->nbOfQueues; k++) {
        if (schedData->queues[k][0] != -1) {
            i = schedData->queues[k][0];
            break;
        }
    }
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                    schedData->queues[k][j] = schedData->queues[k][j+1];
                }
            } else {
                if (tasks[i].quanTime % (quantum * (k + 1)) == 0) {
                    tasks[i].state = READY;
                    tasks[i].quanTime = 1;
                    int temp = schedData->queues[k][0];
                    for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
                        schedData->queues[k][j] = schedData->queues[k][j+1];
                    }
                    for (l = 0; l < MAX_NB_OF_TASKS; l++) {
                        if (schedData->queues[(k + 1) % schedData->nbOfQueues][l] == -1) {
                            schedData->queues[(k + 1) % schedData->nbOfQueues][l] = temp;
                            break;
                        }
                    }
                } else {
                    tasks[i].executionTime++;
                    tasks[i].quanTime++;
                    return i;
                }
            }
        }
    }

    // Otherwise, elect the first task in the queue
    for (k = 0; k < schedData->nbOfQueues; k++) {
        if (schedData->queues[k][0] != -1) {
            i = schedData->queues[k][0];
            break;
        }
    }
    if (i != -1) {
        if (tasks[i].state != TERMINATED) {
            tasks[i].executionTime++;
            tasks[i].state = RUNNING;
        }
        return i;
    }

    // No task could be elected
    return -1;
}


int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime, int quantum) {
    int i, j, k;
    int num_task = 0;

    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        schedData->nbOfQueues = 1;
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[0][i] = -1;
        }
    }

    // Admit new tasks (currentTime >= arrivalTime)
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            tasks[i].waitTime = 0;
            tasks[i].quanTime = 0;
            schedData->queues[0][j] = i;
            j++;
        }
        if (tasks[i].state == READY) {
            tasks[i].waitTime++;
        }
    }
    num_task = j;
    print_queues(tasks, schedData);

    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    for (k = 0; k < nbOfTasks; k++) {
        if (schedData->queues[0][k] != SLEEPING) {
            i = schedData->queues[0][k];
            break;
        }
    }
    int add = 0;
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                if (tasks[i].ioCycle != 0 && tasks[i].executionTime % tasks[i].ioCycle == 0) {
                    tasks[i].state = SLEEPING;
                    tasks[i].ioTime = 0;
                    int temp = schedData->queues[0][k];
                    for (j = k; j < MAX_NB_OF_TASKS - 1; j++) {
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                    }
                    schedData->queues[0][num_task - 1] = temp;
                } else {
                    tasks[i].state = TERMINATED;
                    for (j = k; j < MAX_NB_OF_TASKS - 1; j++) {
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                    }
                }
            } else {
                if (tasks[i].quanTime % quantum == 0) {
                    if (tasks[i].ioCycle != 0 && tasks[i].executionTime % tasks[i].ioCycle == 0) {
                        tasks[i].state = SLEEPING;
                    } else {
                        tasks[i].state = READY;
                    }
                    tasks[i].quanTime = 0;
                    int temp = schedData->queues[0][k];
                    for (j = k; j < MAX_NB_OF_TASKS - 1; j++) {
                        schedData->queues[0][j] = schedData->queues[0][j+1];
                    }
                    schedData->queues[0][num_task - 1] = temp;
                } else {
                    if (tasks[i].ioCycle != 0 && tasks[i].executionTime % tasks[i].ioCycle == 0) {
                        tasks[i].state = SLEEPING;
                        tasks[i].quanTime = 0;
                        tasks[i].ioTime = 0;
                        int temp = schedData->queues[0][k];
                        for (j = k; j < MAX_NB_OF_TASKS - 1; j++) {
                            schedData->queues[0][j] = schedData->queues[0][j+1];
                        }
                        schedData->queues[0][num_task - 1] = temp;
                    } else {
                        tasks[i].executionTime++;
                        tasks[i].quanTime++;
                        add = 1;
                    }
                }
            }
        }
    }
    for (j = 0; j < nbOfTasks; j++) {
        i = schedData->queues[0][j];
        if (tasks[i].state == SLEEPING) {
            if (tasks[i].ioCycle != 0 && tasks[i].ioTime == tasks[i].ioLength) {
                if (tasks[i].executionTime == tasks[i].computationTime) {
                    tasks[i].state = TERMINATED;
                } else {
                    // printf("hey %d\n", i);
                    tasks[i].state = READY;
                }
            } else {
                tasks[i].ioTime++;
            }
        }
    }
    if (add) {
        return schedData->queues[0][k];
    }

    // Otherwise, elect the ·first task in the queue
    i = schedData->queues[0][k];
    if (i != -1) {
        if (tasks[i].state == SLEEPING || tasks[i].state == TERMINATED) {
            return -1;
        }
        tasks[i].executionTime++;
        tasks[i].quanTime++;
        tasks[i].state = RUNNING;
        if (tasks[i].ioLength == 1) {
            tasks[i].state = SLEEPING;
            tasks[i].quanTime = 0;
            tasks[i].ioTime = 0;
            int temp = schedData->queues[0][k];
            for (j = k; j < MAX_NB_OF_TASKS - 1; j++) {
                schedData->queues[0][j] = schedData->queues[0][j+1];
            }
            schedData->queues[0][num_task - 1] = temp;
        }
        return i;
    }

    // No task could be elected
    return -1;
}
