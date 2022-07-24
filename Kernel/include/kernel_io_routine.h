#ifndef KISS_KERNEL_IO_ROUTINE_H
#define KISS_KERNEL_IO_ROUTINE_H


#include "../../Utils/include/common_structures.h"

typedef struct IO_Object {
    uint32_t pid;
    uint32_t block_time;
} t_io_object;

void initialize_io_routine();

void execute_io_routine(t_io_object *io_object);

void free_io_routine();

#endif //KISS_KERNEL_IO_ROUTINE_H
