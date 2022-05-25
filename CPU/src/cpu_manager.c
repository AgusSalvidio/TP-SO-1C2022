#include <cpu_manager.h>
#include <cpu_query_performer.h>
#include "../../Utils/include/common_structures.h"
#include "cpu_logs_manager.h"
#include "cpu_configuration_manager.h"
#include <stdlib.h>
#include <unistd.h>
/*
t_handshake* logical_address_translator;

t_handshake* logic_address_translator(){
    return logical_address_translator;
}*/

void wait_delay_time(){
    uint32_t delay_time_in_seconds = get_noop_delay()/1000;
    sleep(delay_time_in_seconds);
}

t_request* response_request_with(void* received_structure, uint32_t operation){
    t_request* request_to_send = safe_malloc(sizeof(t_request));
    request_to_send -> operation = operation;
    request_to_send -> structure = received_structure;
    request_to_send -> sanitizer_function = free;
    return request_to_send;
}
/*
void* handle_handshake_request_procedure(t_handshake * received_handshake){

    logical_address_translator -> page_size = received_handshake->page_size;
    logical_address_translator -> entries_per_page = received_handshake->entries_per_page;

}*/
void* handle_read_request_procedure(uint32_t received_content){
    //Solo muestro por pantalla el contenido que devuelve memoria
    log_read_content(received_content);
}
void* handle_write_request_procedure(){
    //No hago nada, solo recibo la confirmacion por parte de memoria

}
void* handle_copy_request_procedure(){
    //Idem write, solo recibo la confirmacion por parte de memoria

}
void* handle_no_op_request_procedure(){
    //Tengo que esperar el tiempo especificado en .config
    wait_delay_time();
}
void* handle_IO_request_procedure(uint32_t blocked_time){
    //Retorno el PCB actualizado y el tiempo en milisegundos
    uint32_t time = blocked_time/1000;
    sleep(time);

    //Retornar PCB
}
void* handle_exit_request_procedure(){
    //Retorno PCB actualizado
}
void* handle_PCB_request_procedure(){
    //Devolver PCB actualizado a Kernel
}

void free_cpu_manager(){};