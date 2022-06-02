#include "kernel_long_term_scheduler.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_configuration.h"
#include "kernel_process_image.h"
#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"

sem_t sem_available_slots;
sem_t sem_new_processes;
pthread_mutex_t mutex_process;

void schedule_context() {
    safe_sem_wait(&sem_available_slots);


    safe_mutex_lock(&mutex_process);

    t_pcb * pcb_found = list_first(scheduler_queue_of(NEW)->pcb_list);
    safe_mutex_unlock(&mutex_process);

    t_state_transition *transition = state_transition_for(pcb_found, READY);
    transition->function(pcb_found);

}

void algoritmo_planificador_largo_plazo() {
    safe_sem_initialize_with_value(&sem_available_slots, get_multiprogramming_degree());
    safe_sem_initialize(&sem_new_processes);
    safe_mutex_initialize(&mutex_process);
    while (1) {
        safe_sem_wait(&sem_new_processes);
        schedule_context();
    }
}

void request_schedule_context() {
    safe_mutex_lock(&mutex_process);
    safe_sem_post(&sem_new_processes);
    safe_mutex_unlock(&mutex_process);
}