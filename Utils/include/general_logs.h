#ifndef KISS_GENERAL_LOGS_H
#define KISS_GENERAL_LOGS_H

#include <commons/log.h>
#include "common_structures.h"

void log_succesful_start_up();
void log_successful_execution();
void log_successful_clean_up();
void log_succesful_initialize_pretty_printer();
void log_succesful_initialize_serializable_objects();
void log_syscall_error_with_errno_description(char* message_error);
void log_syscall_error(char* message_error);
void log_send_all_error(int sent_bytes, int amount_of_bytes);
void log_pthread_create_error(unsigned int process_id);
void log_queue_name_not_found_error(uint32_t queue_code);
void log_queue_code_not_found_error(char* queue_name);
void log_printable_object_not_found_error();
void log_about_to_send_request(t_request* request);
void log_request_sent(t_request* request);
void log_request_received(t_log* logger, t_request* request);
void log_unknown_chained_evaluation_type_error();
void log_garbage_to_stop_considering_that_not_found_error();
void log_request_received_basic();
void log_failed_ack_error();
void log_succesful_suscription_to(t_log* logger, uint32_t operation_queue);
void log_errorful_not_existing_log(char* log_name);
void log_expected_to_have_only_one_element_error();
void log_directory_could_not_open_in_path_error(char* path);
void log_file_not_found_error(char* extension);

void log_list_invalid_index_access(int index, int another_index,t_list* self);
#endif //KISS_GENERAL_LOGS_H
