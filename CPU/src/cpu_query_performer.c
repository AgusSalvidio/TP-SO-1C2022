#include <cpu_query_performer.h>
#include <commons/collections/list.h>
#include <cpu_logs_manager.h>
#include <stdlib.h>
#include <cpu_manager.h>
#include "../../Utils/include/common_structures.h"

t_list* query_performers;


void* PCB_query_performer(t_request* request){
    t_pcb* current_pcb = request -> structure;
    return handle_PCB_request_procedure(current_pcb);
}

void initialize_PCB_query_performer(){

    t_query_performer* query_performer = safe_malloc(sizeof(t_query_performer));
    query_performer -> operation = PCB;
    query_performer -> perform_function = PCB_query_performer;
    list_add(query_performers, query_performer);
}

void initialize_cpu_query_performers(){

    query_performers = list_create();

    initialize_PCB_query_performer();

    log_cpu_query_performers_loaded_succesfully();
}

t_query_performer *query_performer_handle(uint32_t operation) {

    bool _can_handle(void *query_performer) {
        t_query_performer *cast_query_performer = (t_query_performer *) query_performer;
        return (*(cast_query_performer->perform_function)) == (operation); //Revisar
    }

    //Me quedo con el Query Performer que puede manejar el mensaje que me llegó
    t_query_performer *query_performer_found = list_find(query_performers, _can_handle);

    //Si no se encontró un Query Performer que pueda manejarlo, logueo y rompo
    if (!query_performer_found) {

        log_query_performer_not_found_error_from_cpu(operation);
        //free_system();
    }

    return query_performer_found;
}

t_query_performer *query_performer_with_code(uint32_t operation_code) {

    bool _equals_operation(void *query_performer) {
        return ((t_query_performer *) query_performer)->operation == operation_code;
    }

    return list_find(query_performers, _equals_operation);
}

void free_cpu_query_performers() {
    list_destroy_and_destroy_elements(query_performers, free);
}