#include <unistd.h>
#include "kernel_mid_term_scheduler.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_event.h"
#include "kernel_configuration.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "kernel_long_term_scheduler.h"
#include "../../Utils/include/pthread_wrapper.h"

sem_t sem_blocked_processes;
pthread_mutex_t suspension_mutex;

void execute_suspension_routine(t_pcb *pcb) {
    sleep_for(get_max_block_time() / 1000);
    bool _find_by_pcb(t_pcb *pcb_to_compare) {
        return pcb->pid == pcb_to_compare->pid;
    };
    safe_mutex_lock(&suspension_mutex);
    if (list_any_satisfy(scheduler_queue_of(BLOCKED)->pcb_list, _find_by_pcb)) {
        t_state_transition *state_transition = state_transition_for(pcb, SUSPENDED_BLOCKED);
        state_transition->function(pcb);
    }
    safe_mutex_unlock(&suspension_mutex);
}

void new_blocked_process() {
    safe_sem_post(&sem_blocked_processes);
}

void algoritmo_planificador_mediano_plazo() {
    safe_sem_initialize(&sem_blocked_processes);
    safe_mutex_initialize(&suspension_mutex);
    subscribe_to_event_doing(PROCESS_BLOCKED, new_blocked_process);
    while (1) {
        safe_sem_wait(&sem_blocked_processes);
        t_pcb *pcb = list_get_last_element(scheduler_queue_of(BLOCKED)->pcb_list);
        pthread_detach(default_safe_thread_create((void *(*)(void *)) execute_suspension_routine, pcb));
    }
}

void free_mid_term_scheduler() {
    safe_sem_destroy(&sem_blocked_processes);
    safe_mutex_destroy(&suspension_mutex);
}