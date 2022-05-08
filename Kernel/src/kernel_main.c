#include <stdlib.h>
#include "../include/planification.h"
#include "../include/structs.h"

void init_kernel(char* pathConfig){
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

void cerrar_kernel() {
    log_destroy(LOGGER);
    config_destroy(CONFIG);

    exit(1);
}

int main(void){
    // init_kernel();

    return EXIT_SUCCESS;
}