#ifndef KISS_KERNEL_LONG_TERM_SCHEDULER_H
#define KISS_KERNEL_LONG_TERM_SCHEDULER_H

#include <stdint.h>

void algoritmo_planificador_largo_plazo();

void request_schedule_process();

void request_process_remove_from_schedule();

uint32_t get_current_available_slots();

void free_long_term_scheduler();

#endif //KISS_KERNEL_LONG_TERM_SCHEDULER_H
