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

t_kernel_config CONFIG_KERNEL;
t_log* LOGGER;

//LISTAS
t_list* LISTA_NEW;
t_list* LISTA_READY;
t_list* LISTA_EXEC;
t_list* LISTA_BLOCKED;
t_list* LISTA_BLOCKED_SUSPENDED;
t_list* LISTA_READY_SUSPENDED;
t_list* LISTA_READY_EXIT;

//MUTEXES
pthread_mutex_t mutex_lista_new;
pthread_mutex_t mutex_lista_ready;
pthread_mutex_t mutex_lista_exec;
pthread_mutex_t mutex_lista_blocked;
pthread_mutex_t mutex_lista_blocked_suspended;
pthread_mutex_t mutex_lista_ready_suspended;
pthread_mutex_t mutex_lista_exit;

//HILOS
pthread_t planificador_corto_plazo;
pthread_t planificador_mediano_plazo;
pthread_t planificador_largo_plazo;

//FUNCIONES
void iniciar_planificador_corto_plazo();
void iniciar_planificador_mediano_plazo();
void iniciar_planificador_largo_plazo();
void algoritmo_planificador_corto_plazo();
void algoritmo_planificador_mediano_plazo();
void algoritmo_planificador_largo_plazo();

int main(void){

    return EXIT_SUCCESS;

}

void iniciar_planificador_corto_plazo() {
    pthread_create(&planificador_corto_plazo, NULL, (void*)algoritmo_planificador_corto_plazo, NULL);
    log_info(LOGGER, "Planificador de corto plazo creado correctamente utilizando el algoritmo '%s'.\n", CONFIG_KERNEL.algoritmo_planificacion);
    pthread_detach(planificador_corto_plazo);
}

void iniciar_planificador_mediano_plazo() {
    pthread_create(&planificador_mediano_plazo, NULL, (void*)algoritmo_planificador_mediano_plazo, NULL);
    log_info(LOGGER, "Planificador de mediano plazo creado correctamente.\n");
    pthread_detach(planificador_mediano_plazo);
}

void iniciar_planificador_largo_plazo() {
    pthread_create(&planificador_largo_plazo, NULL, (void*)algoritmo_planificador_largo_plazo, NULL);
    log_info(LOGGER, "Planificador de largo plazo creado correctamente.\n");
    pthread_detach(planificador_largo_plazo);
}

