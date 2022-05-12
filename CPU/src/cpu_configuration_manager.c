#include <cpu_configuration_manager.h>
#include "../../Utils/include/configuration_manager.h"
#include <stdint.h>

uint32_t tlb_entries;
char* tlb_replacement;
uint32_t noop_delay;
char* memory_ip;
char* memory_port;
char* dispatch_port;
char* interrupt_port;


void initialize_cpu_configuration_manager(){
    initialize_configuration_manager();
    tlb_entries = config_get_int_at("ENTRADAS_TLB");
    tlb_replacement = config_get_string_at("REEMPLAZO_TLB");
    noop_delay = config_get_int_at("RETARDO_NOOP");
    memory_ip = config_get_string_at("IP_MEMORIA");
    memory_port = config_get_string_at("PUERTO_MEMORIA");
    dispatch_port = config_get_string_at("PUERTO_ESCUCHA_DISPATCH");
    interrupt_port = config_get_string_at("PUERTO_ESCUCHA_INTERRUPT");
}

uint32_t get_tlb_entries(){
    return tlb_entries;
}

char* get_tlb_replacement(){
    return tlb_replacement;
}

uint32_t get_noop_delay(){
    return noop_delay;
}

char* get_memory_ip(){
    return memory_ip;
}

char* get_memory_port(){
    return memory_port;
}

char* get_dispatch_port(){
    return dispatch_port;
}

char* get_interrupt_port(){
    return interrupt_port;
}

void free_cpu_configuration_manager(){
    free_configuration_manager();
}

