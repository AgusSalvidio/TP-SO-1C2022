#ifndef KISS_CPU_MEMORY_CONNECTION_HANDLER_H
#define KISS_CPU_MEMORY_CONNECTION_HANDLER_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

t_handshake* get_handshake_information();
void send_handshake_to_memory();
void send_read_to_memory(uint32_t pid, uint32_t logical_address);
char* receive_read_content_from_memory();
void send_write_to_memory(uint32_t pid, uint32_t logical_address, uint32_t value);
void send_copy_to_memory(uint32_t pid, uint32_t logical_address, uint32_t value);
void send_first_access_to_memory(uint32_t type, uint32_t index, uint32_t entry);
char* receive_access_content_from_memory();

#endif //KISS_CPU_MEMORY_CONNECTION_HANDLER_H
