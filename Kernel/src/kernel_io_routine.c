#include "../include/kernel_io_routine.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"
#include "kernel_logs_manager.h"

pthread_mutex_t mutex;

void initialize_io_routine() {
    safe_mutex_initialize(&mutex);
}

void unblock_process(t_pcb *pcb) {
    t_state_transition *state_transition = state_transition_for(pcb, READY);
    state_transition->function(pcb);
    //TODO modificar cuando este la suspension.
}

void execute_io_routine(t_pcb *pcb) {
    safe_mutex_lock(&mutex);
    t_instruction *instruction = list_get(pcb->instructions, pcb->pc);
    uint32_t time = list_get(instruction->operands,0);
    log_io_starting_execution(pcb->pid, time);
    sleep_for(time/1000);
    log_io_finished_execution(pcb->pid);
    safe_mutex_unlock(&mutex);
    unblock_process(pcb);
}

void free_io_routine() {
    safe_mutex_destroy(&mutex);
}