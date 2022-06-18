#include "../../Utils/include/common_structures.h"
#include <stdio.h>
#include <commons/string.h>
#include <unistd.h>
#include <memory_logs_manager.h>
#include "../include/memory_query_performers.h"
#include "../../Utils/include/paths.h"
#include "../../Utils/include/socket.h"
#include <memory_request_handler.h>


t_list *query_performers;

void free_memory_query_performers() {
    list_destroy_and_destroy_elements(query_performers, free);
}

void* handshake_query_performer(t_request* request){

    t_handshake * handshake = ((t_handshake *) request->structure);
    return handle_handshake_request_procedure(handshake);
}

void* new_process_query_performer(t_request* request){

    t_initialize_process * new_process = ((t_initialize_process *) request->structure);
    return handle_new_process_request_procedure(new_process);
}

void* cpu_first_access_query_performer(t_request* request){

    t_mmu_access * first_access = ((t_mmu_access *) request->structure);
    return handle_cpu_first_access_request_procedure(first_access);
}

void* cpu_second_access_query_performer(t_request* request){

    t_mmu_access * second_access = ((t_mmu_access *) request->structure);
    return handle_cpu_second_access_request_procedure(second_access);
}


void initialize_handshake_query_performer(){

    t_query_performer * query_performer = safe_malloc(sizeof(t_query_performer));
    query_performer -> operation = HANDSHAKE;
    query_performer ->perform_function = handshake_query_performer;
    list_add(query_performers, query_performer);
}
void initialize_read_query_performer(){

}
void initialize_write_query_performer(){

}
void initialize_copy_query_performer(){

}

void initialize_new_process_query_performer(){
    t_query_performer * query_performer = safe_malloc(sizeof(t_query_performer));
    query_performer -> operation = INITIALIZE_PROCESS;
    query_performer ->perform_function = new_process_query_performer;
    list_add(query_performers, query_performer);

}
void initialize_suspend_process_query_performer(){}
void initialize_terminate_process_query_performer(){}
void initialize_cpu_first_access_query_performer(){

    t_query_performer * query_performer = safe_malloc(sizeof(t_query_performer));
    query_performer -> operation = FIRST_ACCESS;
    query_performer ->perform_function = cpu_first_access_query_performer;
    list_add(query_performers, query_performer);

}
void initialize_cpu_second_access_query_performer(){

    t_query_performer * query_performer = safe_malloc(sizeof(t_query_performer));
    query_performer -> operation = SECOND_ACCESS;
    query_performer ->perform_function = cpu_second_access_query_performer;
    list_add(query_performers, query_performer);

}

void initialize_memory_query_performers(){

    query_performers = list_create();

    initialize_handshake_query_performer();
    initialize_read_query_performer();
    initialize_write_query_performer();
    initialize_copy_query_performer();
    initialize_new_process_query_performer();
    initialize_suspend_process_query_performer();
    initialize_terminate_process_query_performer();
    initialize_cpu_first_access_query_performer();
    initialize_cpu_second_access_query_performer();

    log_memory_query_performers_loaded_succesfully();

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

        log_query_performer_not_found_error_from_memory(operation);
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

