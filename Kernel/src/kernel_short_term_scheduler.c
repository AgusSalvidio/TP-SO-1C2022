#include "../include/kernel_short_term_scheduler.h"
#include "kernel_scheduling_algorithm.h"
#include "../../Utils/include/t_list_extension.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "kernel_event.h"
#include "kernel_cpu_connection.h"

sem_t sem_processes_ready_to_exec;

void execute_pcb(t_pcb *pcb) {

    t_burst *burst = connect_and_send_pcb_to_cpu(pcb);
    free(burst);
}

void process_added_to_ready(t_pcb* pcb) {
    safe_sem_post(&sem_processes_ready_to_exec);
}

void algoritmo_planificador_corto_plazo() {
    safe_sem_initialize(&sem_processes_ready_to_exec);
    initialize_scheduling_algorithm();
    initialize_cpu_structures();
    subscribe_to_event_doing(PROCESS_READY_TO_EXECUTE, process_added_to_ready);
    while (1) {
        safe_sem_wait(&sem_processes_ready_to_exec);
        log_scheduling_list(scheduler_queue_of(READY)->pcb_list);
        t_pcb *pcb = list_first(scheduler_queue_of(READY)->pcb_list);
        t_state_transition *state_transition = state_transition_for(pcb, EXEC);
        state_transition->function(pcb);
        execute_pcb(pcb);
    }

}

void free_short_term_scheduler() {
    safe_sem_destroy(&sem_processes_ready_to_exec);
    free_cpu_structures();
    free_scheduling_algorithm();
}
