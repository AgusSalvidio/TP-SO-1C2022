#ifndef KISS_KERNEL_SRT_ALGORITHM_H
#define KISS_KERNEL_SRT_ALGORITHM_H

#include "kernel_scheduling_algorithm.h"

typedef struct Burst_estimator{
    t_pcb *pcb;
    double previous_real;
    double current;
}t_burst_estimation;

void initialize_srt_algorithm();

t_scheduling_algorithm* get_srt_algorithm();

#endif //KISS_KERNEL_SRT_ALGORITHM_H
