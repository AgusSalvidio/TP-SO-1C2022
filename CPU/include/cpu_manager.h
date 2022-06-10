#ifndef KISS_CPU_MANAGER_H
#define KISS_CPU_MANAGER_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

void* handle_read_request_procedure(t_list* operands);
void* handle_write_request_procedure(t_list* operands);
void* handle_copy_request_procedure(t_list* operands);
void* handle_no_op_request_procedure(t_list* operands);
void* handle_IO_request_procedure(t_list* operands);
void* handle_exit_request_procedure(t_list* operands);
void* handle_PCB_request_procedure();

void free_cpu_manager();

#endif //KISS_CPU_MANAGER_H
