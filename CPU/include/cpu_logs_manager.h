#ifndef KISS_CPU_LOGS_MANAGER_H
#define KISS_CPU_LOGS_MANAGER_H

#include <stdint.h>

void initialize_cpu_logs_manager();

void log_read_content(uint32_t content);

void log_cpu_query_performers_loaded_succesfully();
void log_query_performer_not_found_error_from_cpu(uint32_t operation);
void log_cpu_action_performers_loaded_succesfully();
void log_handshake_received_succesfully();
void log_tlb_succesfully_created();
void log_current_instruction_running(char* instruction);
void log_return_pcb_to_kernel();

void free_cpu_logs_manager();

#endif //KISS_CPU_LOGS_MANAGER_H
