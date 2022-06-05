#ifndef KISS_KERNEL_SCHEDULER_QUEUES_H
#define KISS_KERNEL_SCHEDULER_QUEUES_H

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/common_structures.h"

typedef struct Queue {
    uint32_t state;
    pthread_mutex_t mutex;
    t_list *contexts;
} t_scheduler_queue;

typedef enum Process_state {
    NEW, READY,
    EXEC, BLOCKED,
    SUSPENDED_BLOCKED, SUSPENDED_READY,
    Q_EXIT
} t_process_state;

void initialize_scheduler_queues();

t_scheduler_queue *scheduler_queue_of(uint32_t state);

t_list *contexts_in(uint32_t state);

uint32_t amount_of_contexts_in(uint32_t state);

void handling_concurrency_do(t_scheduler_queue *queue, void (*function)());

char *name_of_state(uint32_t to_state);

void move_to(t_pcb *pcb, uint32_t to_state);

void move_to_execute();

void add_to_scheduler_queue(t_pcb *pcb, uint32_t to_state);

void remove_from_scheduler_queue(t_pcb *pcb);

void free_scheduler_queues();

#endif //KISS_KERNEL_SCHEDULER_QUEUES_H
