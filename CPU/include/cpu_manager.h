#ifndef KISS_CPU_MANAGER_H
#define KISS_CPU_MANAGER_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

t_request* response_request_with(void* received_structure, uint32_t operation);
void* handle_read_request_procedure(uint32_t pid, t_list* operands);
void* handle_write_request_procedure(uint32_t pid, t_list* operands);
void* handle_copy_request_procedure(uint32_t pid, t_list* operands);
void* handle_no_op_request_procedure(uint32_t pid, t_list* operands);
void* handle_IO_request_procedure(uint32_t pid, t_list* operands);
void* handle_exit_request_procedure(uint32_t pid, t_list* operands);
void* handle_PCB_request_procedure(t_pcb* current_pcb);

void free_cpu_manager();

#endif //KISS_CPU_MANAGER_H
