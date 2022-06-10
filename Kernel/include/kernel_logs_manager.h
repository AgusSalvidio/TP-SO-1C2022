#ifndef KISS_KERNEL_LOGS_MANAGER_H
#define KISS_KERNEL_LOGS_MANAGER_H

#include <stdint.h>

void initialize_kernel_logs_manager();

void free_kernel_logs_manager();

void log_pcb_added_to_new_queue(uint32_t pid);

void log_pcb_new_to_ready_transition(uint32_t pid);

void log_pcb_ready_to_exec_transition(uint32_t pid);

void log_pcb_blocked_to_ready_transition(uint32_t pid);

void log_pcb_blocked_to_suspended_blocked_transition(uint32_t pid);

void log_pcb_suspended_blocked_to_suspended_ready_transition(uint32_t pid);

void log_pcb_suspended_ready_to_ready_transition(uint32_t pid);

void log_pcb_exec_to_blocked_transition(uint32_t pid);

void log_pcb_exec_to_ready_transition(uint32_t pid);

void log_pcb_exec_to_exit_transition(uint32_t pid);

void log_pcb_blocked_to_exit_transition(uint32_t pid);

void log_pcb_suspended_blocked_to_exit_transition(uint32_t pid);

void log_invalid_transition_error();

void log_invalid_state_error();

void log_pcb_not_found_error(uint32_t pid);

void log_scheduling_algorithm_not_found_error_for(char *algorithm_name);

void log_context_executing(uint32_t pid);

void log_io_starting_execution(uint32_t pid, uint32_t millis);

void log_io_finished_execution(uint32_t pid);

#endif //KISS_KERNEL_LOGS_MANAGER_H
