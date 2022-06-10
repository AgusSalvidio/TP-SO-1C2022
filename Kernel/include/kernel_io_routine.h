#ifndef KISS_KERNEL_IO_ROUTINE_H
#define KISS_KERNEL_IO_ROUTINE_H


#include "../../Utils/include/common_structures.h"

void initialize_io_routine();

void execute_io_routine(t_pcb *pcb);

void free_io_routine();

#endif //KISS_KERNEL_IO_ROUTINE_H
