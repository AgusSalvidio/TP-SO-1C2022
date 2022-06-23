#ifndef KISS_KERNEL_MEMORY_CONNECTION_H
#define KISS_KERNEL_MEMORY_CONNECTION_H

#include <stdint.h>

uint32_t connect_and_send_initialize_to_memory(void *structure_to_send);

void connect_and_send_suspend_to_memory(void *structure_to_send);

void connect_and_send_finalize_to_memory(void *structure_to_send);

#endif //KISS_KERNEL_MEMORY_CONNECTION_H
