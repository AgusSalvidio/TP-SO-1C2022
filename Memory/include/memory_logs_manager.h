#ifndef KISS_MEMORY_LOGS_MANAGER_H
#define KISS_MEMORY_LOGS_MANAGER_H
#include <stdint.h>
#include "memory_manager.h"

void initialize_memory_logs_manager();


void log_memory_query_performers_loaded_succesfully();
void log_query_performer_not_found_error_from_memory(uint32_t operation);
void log_swap_file_delete_procedure_description(char* swap_file_path, char* description);
void free_memory_logs_manager();
void log_main_memory_was_successfully_initialized();
void log_memory_table_manager_was_successfully_initialized();
void log_process_context_manager_was_successfully_initialized();
void log_first_level_table_for_process_was_successfully_initialized(uint32_t pid);
void log_second_level_table_for_process_was_successfully_initialized(uint32_t pid);
void log_swap_file_for_process_was_successfully_initialized(uint32_t pid,char* swap_file_path);
void log_process_context_was_successfully_initialized(uint32_t pid);
void log_cannot_initialize_new_process_because(char* description);
void log_main_memory_frames_were_successfully_released();
void log_process_suspension_was_successful(uint32_t pid);
void log_process_finalization_was_successful(uint32_t pid);
void log_handshake_was_sent_succesfully();
void log_read_request_was_handled_successfully();
void log_write_request_was_handled_successfully();
void log_memory_read_at(uint32_t frame,uint32_t offset);
void log_memory_write_at(uint32_t frame,uint32_t offset, uint32_t value_to_write);
void log_cpu_first_access_was_handled_successfully(uint32_t index,uint32_t entry);
void log_cpu_second_access_was_handled_successfully(uint32_t index, uint32_t entry);
void log_cpu_second_access_cannot_be_handled(uint32_t index, uint32_t entry);
void log_page_was_loaded_in_memory_successfully(uint32_t pid, t_page* page);
void log_frame_related_to_page_was_added_to_process_context_successfully(uint32_t frame, uint32_t page_id);
void log_swap_procedure_was_successful(uint32_t pid,uint32_t victim_page_id,uint32_t selected_page_id);


#endif //KISS_MEMORY_LOGS_MANAGER_H
