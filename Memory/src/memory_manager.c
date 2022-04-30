#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory_manager.h>
#include <commons/collections/list.h>
#include <memory_configuration_manager.h>
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

t_request* response_request_with(void* received_structure, uint32_t operation){
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

    uint32_t page_size = received_handshake->page_size;
    uint32_t entries_per_page = received_handshake->entries_per_page;

    t_handshake* handshake_to_send = handshake_configurations();

    t_request * request_to_send;

    request_to_send = response_request_with(handshake_to_send,HANDSHAKE);

    //wait_delay_time();
    return request_to_send;
}

void free_memory_manager(){}
