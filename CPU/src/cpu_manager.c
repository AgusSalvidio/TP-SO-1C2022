#include <cpu_manager.h>
#include <cpu_query_performer.h>
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>

t_handshake* logical_adress_translator;

t_handshake* logic_adress_translator(){
    return logical_adress_translator;
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
    handshake->page_size = 30;
    handshake->entries_per_page = 10;
    return handshake;
}

void* handle_handshake_request_procedure(t_handshake * received_handshake){

    logical_adress_translator -> page_size = received_handshake->page_size;
    logical_adress_translator -> entries_per_page = received_handshake->entries_per_page;

    t_handshake* handshake_to_send = handshake_configurations();

    t_request * request_to_send = response_request_with(handshake_to_send,HANDSHAKE);
    return request_to_send;
}

void free_cpu_manager(){};