//
// Created by Usuario on 5/8/2022.
//

#include "../include/planification.h"

void iniciar_planificador_corto_plazo() {
    pthread_create(&planificador_corto_plazo, NULL, (void*)algoritmo_planificador_corto_plazo, NULL);
    log_info(LOGGER, "Planificador de corto plazo creado correctamente utilizando el algoritmo '%s'.\n", CONFIG_KERNEL.algoritmo_planificacion);
    pthread_detach(planificador_corto_plazo);
    //pthread_join(planificador_corto_plazo, NULL);
}

void iniciar_planificador_mediano_plazo() {
    pthread_create(&planificador_mediano_plazo, NULL, (void*)algoritmo_planificador_mediano_plazo, NULL);
    log_info(LOGGER, "Planificador de mediano plazo creado correctamente utilizando el algoritmo '%s'.\n", CONFIG_KERNEL.algoritmo_planificacion);
    pthread_detach(planificador_mediano_plazo);
    //pthread_join(planificador_mediano_plazo, NULL);
}

void iniciar_planificador_largo_plazo() {
    pthread_create(&planificador_largo_plazo, NULL, (void*)algoritmo_planificador_largo_plazo, NULL);
    log_info(LOGGER, "Planificador de largo plazo creado correctamente.\n");
    pthread_detach(planificador_largo_plazo);
}