#include "../include/kernel_configuration.h"
#include "../../Utils/include/configuration_manager.h"

char* get_memory_ip() {
    return config_get_string_at("IP_MEMORIA");
}

char* get_memory_port() {
    return config_get_string_at("PUERTO_MEMORIA");
}

char* get_cpu_ip() {
    return config_get_string_at("IP_CPU");
}

char* get_cpu_dispatch_port() {
    return config_get_string_at("PUERTO_CPU_DISPATCH");
}

char* get_cpu_interrupt_port() {
    return config_get_string_at("PUERTO_CPU_INTERRUPT");
}

char* get_listening_port() {
    return config_get_string_at("PUERTO_ESCUCHA");
}

char* get_scheduling_algorithm() {
    return config_get_string_at("ALGORITMO_PLANIFICACION");
}

uint32_t get_initial_estimation() {
    return config_get_int_at("ESTIMACION_INICIAL");
}

double get_alpha() {
    return config_get_double_at("ALFA");
}

uint32_t get_multiprogramming_degree() {
    return config_get_int_at("GRADO_MULTIPROGRAMACION");
}

uint32_t get_max_block_time() {
    return config_get_int_at("TIEMPO_MAXIMO_BLOQUEADO");
}