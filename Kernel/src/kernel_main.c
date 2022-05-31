#include <stdlib.h>
#include "../include/planification.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/general_logs.h"
#include "../include/kernel_logs_manager.h"
#include "kernel_console_connection.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"

void init_kernel(){
    LISTA_NEW = list_create();
    LISTA_READY = list_create();
    LISTA_EXEC = list_create();
    LISTA_BLOCKED = list_create();
    LISTA_BLOCKED_SUSPENDED = list_create();
    LISTA_READY_SUSPENDED = list_create();

    pthread_mutex_init(&mutex_lista_new, NULL);
    pthread_mutex_init(&mutex_lista_ready, NULL);

    pthread_mutex_init(&mutex_lista_exec, NULL);
    pthread_mutex_init(&mutex_lista_blocked, NULL);
    pthread_mutex_init(&mutex_lista_blocked_suspended, NULL);
    pthread_mutex_init(&mutex_lista_ready_suspended, NULL);
    pthread_mutex_init(&mutex_lista_exit, NULL);

    iniciar_planificador_corto_plazo();
    iniciar_planificador_mediano_plazo();
    iniciar_planificador_largo_plazo();
}

int main(void) {

    initialize_configuration_manager();
    initialize_kernel_logs_manager();
    initialize_pretty_printer();
    initialize_garbage_collector();
    initialize_serializable_objects();
    initialize_signal_handler();

    initialize_scheduler_queues();
    initialize_state_transitions();
    initialize_kernel_process_image();

    log_succesful_start_up();
    //TODO: invocacion logica principal
    init_kernel();
    execute_connection_handler();
    //
    free_system();
    return EXIT_SUCCESS;
}

void cerrar_kernel() {
    log_destroy(LOGGER);
    config_destroy(CONFIG);

    exit(1);
}

