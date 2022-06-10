#ifndef KISS_CPU_MEMORY_CONNECTION_HANDLER_H
#define KISS_CPU_MEMORY_CONNECTION_HANDLER_H

#include <stdint.h>

void send_handshake_to_memory();
void send_read_to_memory(uint32_t logical_address);
void send_write_to_memory(uint32_t logical_address, uint32_t value);
void send_copy_to_memory(uint32_t logical_address, uint32_t value);

#endif //KISS_CPU_MEMORY_CONNECTION_HANDLER_H
