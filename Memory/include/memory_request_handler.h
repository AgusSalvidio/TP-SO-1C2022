#ifndef KISS_MEMORY_REQUEST_HANDLER_H
#define KISS_MEMORY_REQUEST_HANDLER_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include "../../Utils/include/common_structures.h"

void* handle_handshake_request_procedure(t_handshake * received_handshake);
void* handle_new_process_request_procedure(t_initialize_process* new_process);
void* handle_cpu_first_access_request_procedure(t_mmu_access *first_access);
void* handle_cpu_second_access_request_procedure(t_mmu_access *first_access);
#endif //KISS_MEMORY_REQUEST_HANDLER_H
