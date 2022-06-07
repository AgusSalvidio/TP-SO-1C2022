#include "../include/kernel_short_term_scheduler.h"
#include "kernel_scheduling_algorithm.h"
#include "../../Utils/include/t_list_extension.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "kernel_logs_manager.h"
#include "kernel_event.h"
#include "kernel_cpu_connection.h"

sem_t sem_processes_ready;

void check_next_transition(t_pcb *pcb) {
    t_instruction *instruction = list_get(pcb->instructions, pcb->pc);
    t_state_transition *transition;
    if (instruction->type == IO) {
        transition = state_transition_for(pcb, BLOCKED);
    } else if (instruction->type == EXIT) {
        transition = state_transition_for(pcb, EXIT);
    } else {
        transition = state_transition_for(pcb, READY);
    }
    transition->function(pcb);
}


void execute_pcb(t_pcb *pcb) {
    t_burst *burst = safe_malloc(sizeof(t_burst));
    burst->pcb = pcb;
    burst->start = current_time_in_milliseconds();
    t_pcb *returned_pcb = connect_and_send_pcb_to_cpu(pcb);
    burst->finished = current_time_in_milliseconds();
    pcb->page_table = returned_pcb->page_table;
    pcb->pc = returned_pcb->pc;
    check_next_transition(pcb);
    //Notifico al algoritmo para que reorganice la lista de ready segun su criterio (paso el burst para srt)
    notify_with_argument(CONTEXT_SWITCH, burst);
    
   
    
    
    free(burst);
}

void process_added_to_ready() {
    safe_sem_post(&sem_processes_ready);
}

void algoritmo_planificador_corto_plazo() {
    initialize_scheduling_algorithm();
    subscribe_to_event_doing(NEW_PROCESS_READY_TO_EXECUTE, process_added_to_ready);
    subscribe_to_event_doing(SUSPENDED_PROCESS_READY_TO_EXECUTE, process_added_to_ready);
    subscribe_to_event_doing(BLOCKED_PROCESS_READY_TO_EXECUTE, process_added_to_ready);
    subscribe_to_event_doing(PROCESS_READY_TO_EXECUTE, process_added_to_ready);
    while (1) {
        safe_sem_wait(&sem_processes_ready);
        t_pcb *pcb = list_first(scheduler_queue_of(READY)->pcb_list);
        t_state_transition *state_transition = state_transition_for(pcb, EXEC);
        state_transition->function(pcb);
        execute_pcb(pcb);
    }

}

void free_short_term_scheduler() {
    safe_sem_destroy(&sem_processes_ready);
}