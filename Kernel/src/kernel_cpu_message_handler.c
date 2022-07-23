#include <stdlib.h>
#include "kernel_cpu_message_handler.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/common_structures.h"
#include "kernel_state_transitions.h"
#include "kernel_scheduler_queues.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_event.h"
#include "kernel_io_routine.h"
#include "kernel_scheduling_algorithm.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/garbage_collector.h"


t_list *handlers;
extern sem_t sem_preempt;

void free_pcb (t_pcb * pcb) {
    void _destroy_operands(t_instruction* instruction) {
        list_destroy(instruction->operands);
        free(instruction);
    };

    list_destroy_and_destroy_elements(pcb->instructions, _destroy_operands);
    stop_considering_garbage(pcb);
    free(pcb);
}

void free_io_pcb (t_io_pcb * io_pcb) {
    free_pcb(io_pcb -> pcb);
    stop_considering_garbage(io_pcb);
    free(io_pcb);
}


void next_transition(t_pcb *returned_pcb, t_burst *burst) {
    t_state_transition *transition;
    if (returned_pcb->pc < list_size(returned_pcb->instructions)) {
        transition = state_transition_for(burst->pcb, READY);
        transition->function(burst -> pcb);
        notify_with_argument(UPDATE_CURRENT_PROCESS_ESTIMATION, burst);
        safe_sem_post(&sem_preempt);
    } else {
        transition = state_transition_for(burst->pcb, Q_EXIT);
        transition->function(burst -> pcb);
    }
    burst->pcb->page_table = returned_pcb->page_table;
    burst->pcb->pc = returned_pcb->pc;
    free_pcb(returned_pcb);

}

void block_process(t_io_pcb *returned_io_pcb, t_burst *burst) {
    t_state_transition *transition;
    transition = state_transition_for(burst -> pcb, BLOCKED);
    transition->function(burst -> pcb);
    notify_with_argument(UPDATE_CURRENT_PROCESS_ESTIMATION, burst);
    t_io_object *io_object = safe_malloc(sizeof (t_io_object));
    io_object->pid = returned_io_pcb -> pcb -> pid;
    io_object->block_time = returned_io_pcb -> blocked_time;
    pthread_t io_thread = default_safe_thread_create((void *(*)(void *)) execute_io_routine, io_object);
    pthread_detach(io_thread);
    notify(PROCESS_BLOCKED);
    burst->pcb->page_table = returned_io_pcb->pcb->page_table;
    burst->pcb->pc = returned_io_pcb->pcb->pc;
    free_io_pcb(returned_io_pcb);
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