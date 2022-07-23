#include "../include/kernel_io_routine.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"
#include "kernel_logs_manager.h"

pthread_mutex_t io_mutex;
extern pthread_mutex_t suspension_mutex;

void unblock_process(uint32_t pid) {
    t_state_transition *state_transition;
    bool _find_by_pcb (t_pcb *pcb_to_compare) {
        return pid == pcb_to_compare -> pid;
    };

    safe_mutex_lock(&suspension_mutex);
    t_pcb *pcb = list_find(scheduler_queue_of(SUSPENDED_BLOCKED)->pcb_list, _find_by_pcb);
    if (pcb) {
        state_transition = state_transition_for(pcb, SUSPENDED_READY);
        state_transition -> function (pcb);
    } else {
        pcb = list_find(scheduler_queue_of(BLOCKED)->pcb_list, _find_by_pcb);
        if (pcb) {
            state_transition = state_transition_for(pcb, READY);
            state_transition->function(pcb);
        }
    }
    safe_mutex_unlock(&suspension_mutex);
}

void execute_io_routine(t_io_object *io_object) {
    safe_mutex_lock(&io_mutex);
    log_io_starting_execution(io_object->pid, io_object->block_time);
    sleep_for(io_object->block_time/1000);
    log_io_finished_execution(io_object->pid);
    safe_mutex_unlock(&io_mutex);
    unblock_process(io_object->pid);
    free(io_object);
}

void initialize_io_routine() {
    safe_mutex_initialize(&io_mutex);
}

void free_io_routine() {
    safe_mutex_destroy(&io_mutex);
}