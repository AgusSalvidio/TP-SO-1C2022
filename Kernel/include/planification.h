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
#include "../../Utils/include/common_structures.h"

//FUNCIONES
void iniciar_planificador_corto_plazo();
void iniciar_planificador_mediano_plazo();
void iniciar_planificador_largo_plazo();

void algoritmo_planificador_largo_plazo();
void algoritmo_planificador_mediano_plazo();
void algoritmo_planificador_corto_plazo();

t_pcb* algoritmo_FIFO();
t_pcb* algoritmo_SRT();

void free_planificacion_threads();

#endif //KISS_PLANIFICATION_H
