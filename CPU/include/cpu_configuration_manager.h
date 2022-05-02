#ifndef KISS_CPU_CONFIGURATION_MANGER_H
#define KISS_CPU_CONFIGURATION_MANGER_H

#include <stdint.h>

void initialize_cpu_configuration_manager();
uint32_t get_tlb_entries();
char* get_tlb_replacement();
uint32_t get_noop_time_out();
char* get_memory_ip();
char* get_memory_port();
char* get_dispatch_port();
char* get_interrupt_port();
void free_cpu_configuration_manager()


#endif //KISS_CPU_CONFIGURATION_MANGER_H