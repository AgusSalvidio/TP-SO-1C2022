//
// Created by Usuario on 5/8/2022.
//

#include "../include/planification.h"
#include "../../Utils/include/logger.h"
#include "kernel_configuration.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_console_connection.h"

//HILOS
pthread_t connection_thread;
pthread_t planificador_corto_plazo;
pthread_t planificador_largo_plazo;
pthread_t planificador_mediano_plazo;

void iniciar_planificador_corto_plazo() {
    pthread_create(&planificador_corto_plazo, NULL, (void*)algoritmo_planificador_corto_plazo, NULL);
    log_info(process_execution_logger(), "Planificador de corto plazo creado correctamente utilizando el algoritmo '%s'.\n", get_scheduling_algorithm());
    //pthread_detach(planificador_corto_plazo);
    //pthread_join(planificador_corto_plazo, NULL);
}

void iniciar_planificador_mediano_plazo() {
    pthread_create(&planificador_mediano_plazo, NULL, (void*)algoritmo_planificador_mediano_plazo, NULL);
    log_info(process_execution_logger(), "Planificador de mediano plazo creado correctamente utilizando el algoritmo '%s'.\n", get_scheduling_algorithm());
    //pthread_detach(planificador_mediano_plazo);
    //pthread_join(planificador_mediano_plazo, NULL);
}

void iniciar_planificador_largo_plazo() {
    connection_thread = default_safe_thread_create((void *) execute_connection_handler, NULL);
    planificador_largo_plazo = default_safe_thread_create((void*) algoritmo_planificador_largo_plazo, NULL);
    log_info(process_execution_logger(), "Planificador de largo plazo creado correctamente.\n");
    safe_thread_join(planificador_largo_plazo);
}

void free_planificacion_threads() {
    safe_thread_cancel(planificador_corto_plazo);
    safe_thread_cancel(planificador_mediano_plazo);
    safe_thread_cancel(planificador_largo_plazo);
    safe_thread_cancel(connection_thread);

}