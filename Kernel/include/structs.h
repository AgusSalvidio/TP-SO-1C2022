//
// Created by Usuario on 5/8/2022.
//

#ifndef KISS_STRUCTS_H
#define KISS_STRUCTS_H

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>

int SERVIDOR_KERNEL;
int SERVIDOR_MEMORIA;
int SERVIDOR_CONSOLA;

t_config* CONFIG;
t_log* LOGGER;

//ESTRUCTURAS
typedef struct {
    char* ip_memoria;
    char* puerto_memoria;
    char* op_cpu;
    char* puerto_cpu_dispatch;
    char* puerto_cpu_interrupt;
    char* puerto_escucha;
    char* algoritmo_planificacion;
    uint32_t estimacion_inicial;
    double alfa;
    uint32_t grado_multiprogramacion;
    uint32_t tiempo_maximo_bloqueado;
}t_kernel_config;
t_kernel_config CONFIG_KERNEL;

typedef struct {
    uint32_t id;
    uint32_t tamano;
    t_list* instrucciones;
    uint32_t program_counter;
    // tabla_paginas
    double estimacion_rafaga;
}t_pcb;

typedef enum {
    FIFO = -1,
    SRT = 1
};

#endif //KISS_STRUCTS_H
