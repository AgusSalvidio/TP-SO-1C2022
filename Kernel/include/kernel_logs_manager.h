#ifndef KISS_KERNEL_LOGS_MANAGER_H
#define KISS_KERNEL_LOGS_MANAGER_H

#include <stdint.h>
#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

void initialize_kernel_logs_manager();

void free_kernel_logs_manager();

void log_pcb_added_to_new_queue(uint32_t pid);

void log_pcb_new_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max);

void log_pcb_ready_to_exec_transition(uint32_t pid);

void log_pcb_blocked_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max);

void log_pcb_blocked_to_suspended_blocked_transition(uint32_t pid, uint32_t available, uint32_t max);

void log_pcb_suspended_blocked_to_suspended_ready_transition(uint32_t pid);

void log_pcb_suspended_ready_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max);

void log_pcb_exec_to_blocked_transition(uint32_t pid);

void log_pcb_exec_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max);

void log_pcb_exec_to_exit_transition(uint32_t pid, uint32_t available, uint32_t max);

void log_pcb_blocked_to_exit_transition(uint32_t pid);

void log_pcb_suspended_blocked_to_exit_transition(uint32_t pid);

void log_invalid_transition_error();

void log_invalid_state_error();

void log_pcb_not_found_error(uint32_t pid);

void log_scheduling_algorithm_not_found_error_for(char *algorithm_name);

void log_context_executing(uint32_t pid);

void log_io_starting_execution(uint32_t pid, uint32_t millis);

void log_io_finished_execution(uint32_t pid);

void log_current_available_slots(uint32_t available, uint32_t max);

void log_estimation_list(t_list* pcb_list);

void log_ready_list(t_list* pcb_list);

void log_update_current_estimation(t_pcb* pcb, double previous, double actual);

void log_calculate_estimation(t_pcb* pcb, double est, double real);

#endif //KISS_KERNEL_LOGS_MANAGER_H
