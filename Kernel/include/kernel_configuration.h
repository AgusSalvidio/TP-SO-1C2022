#ifndef KISS_KERNEL_CONFIGURATION_H
#define KISS_KERNEL_CONFIGURATION_H

#include <stdint.h>

char* get_memory_ip();
char* get_memory_port();
char* get_cpu_ip();
char* get_cpu_dispatch_port();
char* get_cpu_interrupt_port();
char* get_listening_port();
char* get_scheduling_algorithm();
uint32_t get_initial_estimation();
double get_alpha();
uint32_t get_multiprogramming_degree();
uint32_t get_max_block_time();

#endif //KISS_KERNEL_CONFIGURATION_H
