#ifndef KISS_MEMORY_LOGS_MANAGER_H
#define KISS_MEMORY_LOGS_MANAGER_H
#include <stdint.h>

void initialize_memory_logs_manager();


void log_memory_query_performers_loaded_succesfully();
void log_query_performer_not_found_error_from_memory(uint32_t operation);
void log_swap_file_delete_procedure_description(char* swap_file_path, char* description);
void free_memory_logs_manager();


#endif //KISS_MEMORY_LOGS_MANAGER_H
