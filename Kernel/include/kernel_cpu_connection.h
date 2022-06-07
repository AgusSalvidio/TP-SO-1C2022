#ifndef KISS_KERNEL_CPU_CONNECTION_H
#define KISS_KERNEL_CPU_CONNECTION_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

t_pcb *connect_and_send_pcb_to_cpu(void *structure_to_send);

void connect_and_send_interruption_to_cpu();

#endif //KISS_KERNEL_CPU_CONNECTION_H
