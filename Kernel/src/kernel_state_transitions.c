#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_sanitizer.h"
#include "kernel_logs_manager.h"

t_list *state_transitions;

void new_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_new_to_ready_transition(pcb->pid);
    //TODO
}

void ready_to_exec_transition(t_pcb *pcb) {
    move_to(pcb, EXEC);
    log_pcb_ready_to_exec_transition(pcb->pid);
    //TODO
}

void blocked_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_blocked_to_ready_transition(pcb->pid);
    //TODO
}

void blocked_to_suspended_blocked_transition(t_pcb *pcb) {
    move_to(pcb, SUSPENDED_BLOCKED);
    log_pcb_blocked_to_suspended_blocked_transition(pcb->pid);
    //TODO
}

void suspended_blocked_to_suspended_ready_transition(t_pcb *pcb) {
    move_to(pcb, SUSPENDED_READY);
    log_pcb_suspended_blocked_to_suspended_ready_transition(pcb->pid);
}

void suspended_ready_to_ready_transition(t_pcb *pcb) {
    move_to(pcb, READY);
    log_pcb_suspended_ready_to_ready_transition(pcb->pid);
    //TODO
}

void exec_to_blocked_transition(t_pcb *pcb) {
    move_to(pcb, BLOCKED);
    log_pcb_exec_to_blocked_transition(pcb->pid);
    //TODO
}

void exec_to_exit_transition(t_pcb *pcb) {
    move_to(pcb, EXIT);
    log_pcb_exec_to_exit_transition(pcb->pid);
    //TODO
}

void blocked_to_exit_transition(t_pcb *pcb) {
    move_to(pcb, EXIT);
    log_pcb_blocked_to_exit_transition(pcb->pid);
    //TODO
}

void suspended_blocked_to_exit_transition(t_pcb *pcb) {
    move_to(pcb, EXIT);
    log_pcb_suspended_blocked_to_exit_transition(pcb->pid);
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
    initialize_and_load_state_transition(BLOCKED, READY, (void (*)(void *)) blocked_to_ready_transition);
    initialize_and_load_state_transition(BLOCKED, SUSPENDED_BLOCKED,
                                         (void (*)(void *)) blocked_to_suspended_blocked_transition);
    initialize_and_load_state_transition(SUSPENDED_BLOCKED, SUSPENDED_READY,
                                         (void (*)(void *)) suspended_blocked_to_suspended_ready_transition);
    initialize_and_load_state_transition(SUSPENDED_READY, READY,
                                         (void (*)(void *)) suspended_ready_to_ready_transition);
    initialize_and_load_state_transition(EXEC, EXIT, (void (*)(void *)) exec_to_exit_transition);
    initialize_and_load_state_transition(BLOCKED, EXIT, (void (*)(void *)) blocked_to_exit_transition);
    initialize_and_load_state_transition(SUSPENDED_BLOCKED, EXIT,
                                         (void (*)(void *)) suspended_blocked_to_exit_transition);
}

void free_state_transitions() {
    list_destroy_and_destroy_elements(state_transitions, free);
}

t_state_transition *state_transition_for(t_process_image *process, uint32_t to_state) {

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
