#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory_request_handler.h>
#include <commons/collections/list.h>
#include <memory_configuration_manager.h>
#include <memory_manager.h>
#include <commons/string.h>
#include <unistd.h>
#include "../../Utils/include/common_structures.h"
#include "../include/memory_logs_manager.h"
#include "../../Utils/include/t_list_extension.h"

uint32_t maximum_frames_per_process;

uint32_t max_frames_per_process(){
    return maximum_frames_per_process;
}

void wait_delay_time(){
    uint32_t delay_time_in_seconds = swap_time()/1000;
    sleep(delay_time_in_seconds);

}

t_request_response* request_response_using(uint32_t content,char* type_description){
    t_request_response* request_response = safe_malloc(sizeof(t_request_response));
    request_response->content = content;
    request_response->type_description = type_description;
    return request_response;
}

t_request* request_to_send_using(void* received_structure, uint32_t operation){
    t_request* request_to_send = safe_malloc(sizeof(t_request));
    request_to_send -> operation = operation;
    request_to_send -> structure = received_structure;
    request_to_send -> sanitizer_function = free;
    return request_to_send;
}

t_handshake* handshake_configurations(){

    t_handshake* handshake = safe_malloc(sizeof(t_handshake));
    handshake->page_size = page_size_getter();
    handshake->entries_per_page = entries_per_table_getter();
    return handshake;
}

void* handle_handshake_request_procedure(t_handshake * received_handshake){

    t_handshake* handshake_to_send = handshake_configurations();

    t_request * request_to_send;

    request_to_send = request_to_send_using(handshake_to_send, HANDSHAKE);

    //wait_delay_time();
    return request_to_send;
}

void* handle_cpu_first_access_request_procedure(t_mmu_access *first_access){

    t_request_response *second_level_table_index_request_to_send = request_response_using(second_level_table_index_at(first_access->index, first_access->entry), "SUCCESS");

    t_request * request_to_send;
    request_to_send = request_to_send_using(second_level_table_index_request_to_send, REQUEST_RESPONSE);

    wait_delay_time();
    return request_to_send;

}

void* handle_cpu_second_access_request_procedure(t_mmu_access *second_access){

    t_request_response *frame_request_to_send = request_response_using(
            frame_at(second_access->index, second_access->entry), "SUCCESS");

    t_request * request_to_send;
    request_to_send = request_to_send_using(frame_request_to_send, REQUEST_RESPONSE);

    wait_delay_time();
    return request_to_send;

}


void* handle_new_process_request_procedure(t_initialize_process* new_process_received){
    t_initialize_process* new_process = new_process_received;
    uint32_t pid = new_process->pid;
    uint32_t process_size = new_process->process_size;
    t_request *request_to_send;
    uint32_t process_page_quantity = process_size / page_size_getter();

    if(process_page_quantity <= max_page_quantity()) {

        initialize_new_process(pid, process_page_quantity);

        t_request_response *first_level_table_index_request_to_send = request_response_using(table_index_for(pid),"SUCCESS");
        request_to_send = request_to_send_using(first_level_table_index_request_to_send, REQUEST_RESPONSE);
    }
    else{
        t_request_response *cannot_initialize_new_process_request_to_send = request_response_using(pid,"ERROR");
        request_to_send = request_to_send_using(cannot_initialize_new_process_request_to_send, REQUEST_RESPONSE);
    }

    return request_to_send;
}


void* handle_suspend_process_request_procedure(t_suspend_process* process_to_suspend){

    t_suspend_process * suspended_process = process_to_suspend;
    uint32_t pid = suspended_process->pid;

    suspend_process(pid);

    t_request *request_to_send;
    t_request_response *suspended_process_request_response = request_response_using(pid, "SUCCESS");
    request_to_send = request_to_send_using(suspended_process_request_response, REQUEST_RESPONSE);

    return request_to_send;

}

void* handle_finalize_process_request_procedure(t_finalize_process * process_to_finalize){

    t_finalize_process * finalized_process = process_to_finalize;
    uint32_t pid = finalized_process->pid;

    finalize_process(pid);

    t_request *request_to_send;
    t_request_response *finalized_process_request_response = request_response_using(pid, "SUCCESS");
    request_to_send = request_to_send_using(finalized_process_request_response, REQUEST_RESPONSE);

    return request_to_send;


}


void* handle_read_request_procedure(t_read *read_request){

    t_physical_address* physical_address = read_request->physical_address;

    t_request_response* read_value_request_to_send = request_response_using(read_value_from(physical_address), "SUCCESS");

    t_request * request_to_send;
    request_to_send = request_to_send_using(read_value_request_to_send, REQUEST_RESPONSE);

    wait_delay_time();
    return request_to_send;
}

void* handle_write_request_procedure(t_write* write_request){

    t_physical_address* physical_address = write_request->physical_address;
    uint32_t value_to_write = write_request->value;

    t_request_response* write_value_request_to_send = request_response_using(write_value_on(physical_address,value_to_write), "SUCCESS");

    t_request * request_to_send;
    request_to_send = request_to_send_using(write_value_request_to_send, REQUEST_RESPONSE);

    wait_delay_time();
    return request_to_send;

}


void free_memory_manager(){}
