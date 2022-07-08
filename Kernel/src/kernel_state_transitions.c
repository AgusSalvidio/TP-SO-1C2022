#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_sanitizer.h"
#include "kernel_logs_manager.h"
#include "kernel_memory_connection.h"
#include "kernel_event.h"
#include "kernel_io_routine.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_long_term_scheduler.h"

t_list *state_transitions;

void new_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_new_to_ready_transition(pcb->pid);
    t_initialize_process *initialize_process = safe_malloc(sizeof(t_initialize_process));
    initialize_process->pid = pcb ->pid;
    initialize_process->process_size = pcb->process_size;
    uint32_t page_table_id = connect_and_send_initialize_to_memory(initialize_process);
    pcb->page_table = page_table_id;
    notify(SEND_INTERRUPTION_SIGNAL);
    notify(PROCESS_READY_TO_EXECUTE);
}

void ready_to_exec_transition(t_pcb *pcb) {
    move_to(pcb, EXEC);
    log_pcb_ready_to_exec_transition(pcb->pid);
}

void blocked_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_blocked_to_ready_transition(pcb->pid);
    notify(SEND_INTERRUPTION_SIGNAL);
    notify(PROCESS_READY_TO_EXECUTE);
}

void blocked_to_suspended_blocked_transition(t_pcb *pcb) {
    move_to(pcb, SUSPENDED_BLOCKED);
    t_suspend_process *suspend_process = safe_malloc(sizeof(t_suspend_process));
    suspend_process->pid = pcb->pid;
    connect_and_send_suspend_to_memory(suspend_process);
    log_pcb_blocked_to_suspended_blocked_transition(pcb->pid);
    request_process_remove_from_schedule();
}

void suspended_blocked_to_suspended_ready_transition(t_pcb *pcb) {
    move_to(pcb, SUSPENDED_READY);
    log_pcb_suspended_blocked_to_suspended_ready_transition(pcb->pid);
    request_schedule_process();
}

void suspended_ready_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_suspended_ready_to_ready_transition(pcb->pid);
    notify(SEND_INTERRUPTION_SIGNAL);
    notify(PROCESS_READY_TO_EXECUTE);
}

void exec_to_blocked_transition(t_pcb *pcb) {
    move_to(pcb, BLOCKED);
    log_pcb_exec_to_blocked_transition(pcb->pid);
}

void exec_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_exec_to_ready_transition(pcb->pid);
    notify(PROCESS_READY_TO_EXECUTE);
}

void exec_to_exit_transition(t_pcb *pcb) {
    move_to(pcb, Q_EXIT);
    log_pcb_exec_to_exit_transition(pcb->pid);
    t_finalize_process *finalize_process = safe_malloc(sizeof(t_finalize_process));
    finalize_process->pid = pcb->pid;
    connect_and_send_finalize_to_memory(finalize_process);
    request_process_remove_from_schedule();
}

void initialize_and_load_state_transition(uint32_t from_state, uint32_t to_state, void (*function)(void *)) {
    t_state_transition *state_transition = safe_malloc(sizeof(t_state_transition));
    state_transition->from_state = from_state;
    state_transition->to_state = to_state;
    state_transition->function = function;

    list_add(state_transitions, state_transition);
}

void initialize_state_transitions() {
    state_transitions = list_create();
    initialize_and_load_state_transition(NEW, READY, (void (*)(void *)) new_to_ready_transition);
    initialize_and_load_state_transition(READY, EXEC, (void (*)(void *)) ready_to_exec_transition);
    initialize_and_load_state_transition(EXEC, BLOCKED, (void (*)(void *)) exec_to_blocked_transition);
    initialize_and_load_state_transition(EXEC, READY, (void (*)(void *)) exec_to_ready_transition);
    initialize_and_load_state_transition(BLOCKED, READY, (void (*)(void *)) blocked_to_ready_transition);
    initialize_and_load_state_transition(BLOCKED, SUSPENDED_BLOCKED,
                                         (void (*)(void *)) blocked_to_suspended_blocked_transition);
    initialize_and_load_state_transition(SUSPENDED_BLOCKED, SUSPENDED_READY,
                                         (void (*)(void *)) suspended_blocked_to_suspended_ready_transition);
    initialize_and_load_state_transition(SUSPENDED_READY, READY,
                                         (void (*)(void *)) suspended_ready_to_ready_transition);
    initialize_and_load_state_transition(EXEC, Q_EXIT, (void (*)(void *)) exec_to_exit_transition);
}

void free_state_transitions() {
    list_destroy_and_destroy_elements(state_transitions, free);
}

t_state_transition *state_transition_for(t_pcb *pcb, uint32_t to_state) {

    t_process_image *process = find_process_image_by_pid(pcb->pid);

    bool _is_for(t_state_transition *state_transition) {
        return
                state_transition->to_state == to_state &&
                state_transition->from_state == process->state;
    }

    t_state_transition *transition_found = list_find(state_transitions, _is_for);

    if (!transition_found) {
        log_invalid_transition_error();
        free_system();
    }

    return transition_found;
}
