#ifndef KISS_CPU_KERNEL_INTERRUPT_CONNECTION_HANDLER_H
#define KISS_CPU_KERNEL_INTERRUPT_CONNECTION_HANDLER_H

#include <stdbool.h>

bool current_interruption_status();
void modify_interruption_status();
void initialize_cpu_interrupt_threads();
void free_interrupt_thread();

#endif //KISS_CPU_KERNEL_INTERRUPT_CONNECTION_HANDLER_H
