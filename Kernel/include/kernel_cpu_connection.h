#ifndef KISS_KERNEL_CPU_CONNECTION_H
#define KISS_KERNEL_CPU_CONNECTION_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"
#include "kernel_scheduling_algorithm.h"

t_burst *connect_and_send_pcb_to_cpu(t_pcb *pcb);

void connect_and_send_interruption_to_cpu();

#endif //KISS_KERNEL_CPU_CONNECTION_H
