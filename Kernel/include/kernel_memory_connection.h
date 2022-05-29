#ifndef KISS_KERNEL_MEMORY_CONNECTION_H
#define KISS_KERNEL_MEMORY_CONNECTION_H

#include <stdint.h>

void connect_and_send_to_memory(uint32_t operation, void *structure_to_send);

#endif //KISS_KERNEL_MEMORY_CONNECTION_H
