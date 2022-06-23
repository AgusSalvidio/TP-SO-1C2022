
#ifndef KISS_MEMORY_CONFIGURATION_MANAGER_H
#define KISS_MEMORY_CONFIGURATION_MANAGER_H

#include <stdint.h>
#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>

void initialize_memory_configuration_manager();

char* port_getter();
uint32_t memory_size_getter();
uint32_t page_size_getter();
uint32_t entries_per_table_getter();
uint32_t memory_time();
char* algorithm();
uint32_t swap_time();
uint32_t frames_per_process_getter();
char* swap_path();
void free_memory_configuration_manager();
uint32_t max_page_quantity();
uint32_t quantity_memory_frames();

#endif //KISS_MEMORY_CONFIGURATION_MANAGER_H
