#ifndef KISS_KERNEL_PROCESS_IMAGE_H
#define KISS_KERNEL_PROCESS_IMAGE_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

typedef struct Process_image {
    uint32_t state;
    t_pcb *pcb;
} t_process_image;

void initialize_kernel_process_image();

void create_process_image(t_console_message* console_message);

t_process_image *find_process_image_by_pid(uint32_t pid);

void free_kernel_process_image();

#endif //KISS_KERNEL_PROCESS_IMAGE_H
