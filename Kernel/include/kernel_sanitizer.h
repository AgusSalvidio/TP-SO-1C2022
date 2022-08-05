#ifndef KISS_KERNEL_SANITIZER_H
#define KISS_KERNEL_SANITIZER_H

#include "../../Utils/include/common_structures.h"

void free_pcb(t_pcb *pcb);

void free_io_pcb(t_io_pcb *io_pcb);

void free_system();

#endif //KISS_KERNEL_SANITIZER_H
