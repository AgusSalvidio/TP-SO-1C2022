#include <cpu_manager.h>
#include <cpu_query_performer.h>
#include "../../Utils/include/common_structures.h"
#include "cpu_logs_manager.h"
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
/*
t_handshake* read_test(){

    t_read* read_test = safe_malloc(sizeof(t_read));
    read_test->logical_adress = 5;
    return read_test;
}*/

void* handle_handshake_request_procedure(t_handshake * received_handshake){

    logical_adress_translator -> page_size = received_handshake->page_size;
    logical_adress_translator -> entries_per_page = received_handshake->entries_per_page;

}
void* handle_read_request_procedure(uint32_t received_content){
    log_read_content(received_content);
/*
    t_handshake* read_to_send = read_test();
    t_request * request_to_send = response_request_with(read_to_send,READ);
    return request_to_send;*/
}
void* handle_write_request_procedure(uint32_t received_content){

}
void* handle_copy_request_procedure(uint32_t received_content){

}

void free_cpu_manager(){};