#include "kernel_long_term_scheduler.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_configuration.h"
#include "kernel_process_image.h"
#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"

sem_t sem_available_slots;
sem_t sem_processes;
pthread_mutex_t mutex_process;

void schedule_process() {
    safe_sem_wait(&sem_available_slots);

    safe_mutex_lock(&mutex_process);
    t_pcb * pcb;
    if (list_size(scheduler_queue_of(SUSPENDED_READY)->pcb_list) > 0) {
        pcb = list_first(scheduler_queue_of(SUSPENDED_READY)->pcb_list);
    } else {
        pcb = list_first(scheduler_queue_of(NEW)->pcb_list);
    }
    safe_mutex_unlock(&mutex_process);

    t_state_transition *transition = state_transition_for(pcb, READY);
    transition->function(pcb);
}

void algoritmo_planificador_largo_plazo() {
    safe_sem_initialize_with_value(&sem_available_slots, get_multiprogramming_degree());
    safe_sem_initialize(&sem_processes);
    safe_mutex_initialize(&mutex_process);
    while (1) {
        safe_sem_wait(&sem_processes);
        schedule_process();
    }
}

void request_schedule_process() {
    safe_mutex_lock(&mutex_process);
    safe_sem_post(&sem_processes);
    safe_mutex_unlock(&mutex_process);
}

void request_process_remove_from_schedule() {
    safe_mutex_lock(&mutex_process);
    safe_sem_post(&sem_available_slots);
    safe_mutex_unlock(&mutex_process);
}

void free_long_term_scheduler() {
    safe_sem_destroy(&sem_available_slots);
    safe_sem_destroy(&sem_processes);
    safe_mutex_destroy(&mutex_process);
}