#ifndef KISS_CPU_MANAGER_H
#define KISS_CPU_MANAGER_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

void* handle_handshake_request_procedure(t_handshake * received_handshake);

void free_cpu_manager();

#endif //KISS_CPU_MANAGER_H
