#ifndef KISS_KERNEL_CPU_MESSAGE_HANDLER_H
#define KISS_KERNEL_CPU_MESSAGE_HANDLER_H

#include <stdint.h>

typedef struct Message_handler{
    uint32_t operation;
    void* (*perform_function) (void* request_structure, void* pcb);
}t_message_handler;

t_message_handler * message_handler_for_operation(uint32_t operation_code);
void initialize_cpu_message_handler();
void free_cpu_message_handler();

#endif //KISS_KERNEL_CPU_MESSAGE_HANDLER_H
