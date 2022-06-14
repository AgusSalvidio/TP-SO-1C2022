#include <unistd.h>
#include "kernel_mid_term_scheduler.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_event.h"
#include "kernel_configuration.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"

sem_t suspended_ready_processes;

void new_blocked_process(t_pcb *pcb) {
    sleep_for(get_max_block_time()/1000);
    bool _find_by_pcb (t_pcb *pcb_to_compare) {
        return pcb -> pid == pcb_to_compare -> pid;
    };
    if (list_any_satisfy(scheduler_queue_of(BLOCKED)->pcb_list, _find_by_pcb)) {
        t_state_transition *state_transition = state_transition_for(pcb, SUSPENDED_BLOCKED);
        state_transition->function(pcb);
    }
}

void suspended_process_ready() {
    safe_sem_post(&suspended_ready_processes);
}

void algoritmo_planificador_mediano_plazo() {
    subscribe_to_event_doing(PROCESS_BLOCKED, (void (*)(void *)) new_blocked_process);
    subscribe_to_event_doing(SUSPENDED_PROCESS_READY, suspended_process_ready);
    safe_sem_initialize(&suspended_ready_processes);
    while (1) {
        safe_sem_wait(&suspended_ready_processes);

    }
}

void free_mid_term_scheduler() {
    safe_sem_destroy(&suspended_ready_processes);
}