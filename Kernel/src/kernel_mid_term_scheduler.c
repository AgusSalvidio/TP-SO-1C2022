#include <unistd.h>
#include <commons/string.h>
#include "kernel_mid_term_scheduler.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_event.h"
#include "kernel_configuration.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "kernel_long_term_scheduler.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/logger.h"
#include "kernel_logs_manager.h"

sem_t sem_blocked_processes;
pthread_mutex_t suspension_mutex;
t_list* process_to_suspend;

void execute_suspension_routine(uint32_t pid) {

    bool _find_by_pcb(t_pcb *pcb_to_compare) {
        return pid == pcb_to_compare->pid;
    };
    safe_mutex_lock(&suspension_mutex);
    t_pcb * pcb = list_find(scheduler_queue_of(BLOCKED)->pcb_list, _find_by_pcb);
    if (pcb) {
        t_state_transition *state_transition = state_transition_for(pcb, SUSPENDED_BLOCKED);
        state_transition->function(pcb);
    }
    safe_mutex_unlock(&suspension_mutex);
}

void new_blocked_process(uint32_t pid) {
    list_add(process_to_suspend, pid);
    safe_sem_post(&sem_blocked_processes);
}

void algoritmo_planificador_mediano_plazo() {
    safe_sem_initialize(&sem_blocked_processes);
    safe_mutex_initialize(&suspension_mutex);
    subscribe_to_event_doing(PROCESS_BLOCKED, new_blocked_process);
    process_to_suspend = list_create();
    while (1) {
        safe_sem_wait(&sem_blocked_processes);
        uint32_t pid = list_remove_first(process_to_suspend);
        execute_suspension_routine(pid);
    }
}

void free_mid_term_scheduler() {
    safe_sem_destroy(&sem_blocked_processes);
    safe_mutex_destroy(&suspension_mutex);
    list_destroy(process_to_suspend);
}