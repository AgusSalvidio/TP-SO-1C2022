#ifndef KISS_KERNEL_SCHEDULING_ALGORITHM_H
#define KISS_KERNEL_SCHEDULING_ALGORITHM_H

#include "../../Utils/include/common_structures.h"

typedef struct Scheduling_algorithm{
    char* algorithm_name;
    void (*update_ready_queue_when_adding_function) (t_pcb* pcb);
    void (*resolve_dependencies_function) ();
    void (*print_estimation_function) ();
}t_scheduling_algorithm;

typedef struct Burst_context{
    t_pcb *pcb;
    uint32_t start;
    uint32_t finished;
}t_burst;

void initialize_scheduling_algorithm();

void free_scheduling_algorithm();

#endif //KISS_KERNEL_SCHEDULING_ALGORITHM_H
