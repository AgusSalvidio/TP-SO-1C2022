//
// Created by Usuario on 5/8/2022.
//

#ifndef KISS_PLANIFICATION_H
#define KISS_PLANIFICATION_H

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include "../include/structs.h"

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

//FUNCIONES
void iniciar_planificador_corto_plazo();
void iniciar_planificador_mediano_plazo();
void iniciar_planificador_largo_plazo();

void algoritmo_planificador_largo_plazo();
void algoritmo_planificador_mediano_plazo();
void algoritmo_planificador_corto_plazo();

t_pcb* algoritmo_FIFO();
t_pcb* algoritmo_SRT();

#endif //KISS_PLANIFICATION_H
