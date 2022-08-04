#ifndef KISS_CPU_MEMORY_CONNECTION_HANDLER_H
#define KISS_CPU_MEMORY_CONNECTION_HANDLER_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

t_handshake* get_handshake_information();
void send_handshake_to_memory();
uint32_t send_read_to_memory(t_physical_address* physical_address);
void send_write_to_memory(t_physical_address* physical_address, uint32_t value);
void send_copy_to_memory(t_physical_address* physical_address, uint32_t value);
uint32_t send_mmu_access_to_memory(uint32_t type, uint32_t index, uint32_t entry);

#endif //KISS_CPU_MEMORY_CONNECTION_HANDLER_H
