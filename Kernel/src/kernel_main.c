#include <stdlib.h>

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
    int estimacion_inicial;
    double alfa;
    int grado_multiprogramacion;
    int tiempo_maximo_bloqueado;
}t_kernel_config;

typedef struct {
    int id;
    int tamano;
    t_list* instrucciones;
    int program_counter;
    // tabla_paginas
    double estimacion_rafaga;
}t_pcb;

typedef enum {
    FIFO = -1,
    SRT = 1,
};

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
pthread_t planificador_largo_plazo;
pthread_t planificador_mediano_plazo;

int main(void){

    return EXIT_SUCCESS;

}

