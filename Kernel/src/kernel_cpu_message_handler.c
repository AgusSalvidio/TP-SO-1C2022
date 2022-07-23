#include <stdlib.h>
#include "kernel_cpu_message_handler.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_event.h"
#include "kernel_io_routine.h"


t_list *handlers;

t_pcb *next_transition(t_pcb *returned_pcb, t_pcb *pcb) {
    t_state_transition *transition;
    if (returned_pcb->pc < list_size(returned_pcb->instructions)) {
        transition = state_transition_for(pcb, READY);
    } else {
        transition = state_transition_for(pcb, Q_EXIT);
    }
    transition->function(pcb);
    return returned_pcb;
}

t_pcb *block_process(t_io_pcb *returned_io_pcb, t_pcb* pcb) {
    t_state_transition *transition;
    transition = state_transition_for(pcb, BLOCKED);
    transition->function(pcb);
    default_safe_thread_create((void *(*)(void *)) execute_io_routine, returned_io_pcb);
    notify(PROCESS_BLOCKED);
    return returned_io_pcb->pcb;
}

void initialize_and_load_pcb_action_performer() {
    t_message_handler *message_handler = safe_malloc(sizeof(t_message_handler));
    message_handler->operation = PCB;
    message_handler->perform_function = next_transition;

    list_add(handlers, message_handler);
}

void initialize_and_load_io_action_performer() {
    t_message_handler *message_handler = safe_malloc(sizeof(t_message_handler));
    message_handler->operation = IO_PCB;
    message_handler->perform_function = block_process;

    list_add(handlers, message_handler);
}

void initialize_cpu_message_handler() {

    handlers = list_create();
    initialize_and_load_pcb_action_performer();
    initialize_and_load_io_action_performer();

}

t_message_handler *message_handler_for_operation(uint32_t operation_code) {

    bool _equals_operation(void *message_handler) {
        return ((t_message_handler *) message_handler)->operation == operation_code;
    }

    return list_find(handlers, _equals_operation);
}

void free_cpu_message_handler() {
    list_destroy_and_destroy_elements(handlers, free);
}