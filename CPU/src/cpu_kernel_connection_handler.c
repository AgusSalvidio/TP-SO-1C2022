#include <cpu_kernel_connection_handler.h>
#include <stdint.h>
#include <stdlib.h>
#include <cpu_configuration_manager.h>
#include "../../Utils/include/socket.h"

uint32_t* dispatch_socket;
uint32_t* interrupt_socket;

void initialize_cpu_kernel_dispatch_connection_handler(){
    dispatch_socket = listen_at(get_dispatch_port());
}

void initialize_cpu_kernel_interrupt_connection_handler(){
    interrupt_socket = listen_at(get_interrupt_port());
}

void free_cpu_kernel_connection_handler(){
    free(dispatch_socket);
    free(interrupt_socket);
}
