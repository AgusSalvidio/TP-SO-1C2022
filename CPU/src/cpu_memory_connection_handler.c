#include <cpu_memory_connection_handler.h>
#include <stdint.h>
#include <stdlib.h>
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "cpu_logs_manager.h"


t_handshake* logical_address_translator;

t_connection_information* connect_to_memory() {

    char* ip = config_get_string_at("IP_MEMORIA");
    char* port = config_get_string_at("PUERTO_MEMORIA");

    t_connection_information *conn_info = connect_to(ip, port);

    return conn_info;
}

t_handshake* get_logical_address_translator(){
    return logical_address_translator;
}

void send_handshake_to_memory(){

    t_connection_information *memory_conn = connect_to_memory();

    t_handshake * handshake = safe_malloc(sizeof(t_handshake));
    handshake->page_size = 1;
    handshake->entries_per_page = 1;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = HANDSHAKE;
    request -> structure = handshake;
    request -> sanitizer_function = free;

    serialize_and_send_structure(request, memory_conn -> socket_fd);
    free_request(request);

    t_request* response = receive_and_deserialize_structure(memory_conn -> socket_fd);

    logical_address_translator = safe_malloc(sizeof (t_handshake));
    logical_address_translator = (t_handshake*) response -> structure;

    free_request(response);

    free_and_close_connection_information(memory_conn);
}

void send_read_to_memory(uint32_t logical_address){
    t_connection_information *memory_conn = connect_to_memory();

    t_read* read = safe_malloc(sizeof(t_read));
    read -> logical_address = logical_address;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = READ;
    request -> structure = read;
    request -> sanitizer_function = free;

    serialize_and_send_structure(request, memory_conn -> socket_fd);
    free_request(request);

    t_request* response = receive_and_deserialize_structure(memory_conn -> socket_fd);

    t_request_response* content = safe_malloc(sizeof (t_request_response));
    content = (t_request_response*) response -> structure;
    log_read_content(content->content);

    free_request(response);
    free_and_close_connection_information(memory_conn);
}

void send_write_to_memory(uint32_t logical_address, uint32_t value){
    t_connection_information *memory_conn = connect_to_memory();

    t_write* write = safe_malloc(sizeof(t_write));
    write -> logical_address = logical_address;
    write -> value = value;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = WRITE;
    request -> structure = write;
    request -> sanitizer_function = free;

    serialize_and_send_structure(request, memory_conn -> socket_fd);
    free_request(request);

    free_and_close_connection_information(memory_conn);
}

void send_copy_to_memory(uint32_t destiny_logical_address, uint32_t value){
    t_connection_information *memory_conn = connect_to_memory();

    t_copy* copy = safe_malloc(sizeof(t_copy));
    copy -> logical_address = destiny_logical_address;
    copy -> value = value;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = COPY;
    request -> structure = copy;
    request -> sanitizer_function = free;

    serialize_and_send_structure(request, memory_conn -> socket_fd);

    free_request(request);
    free_and_close_connection_information(memory_conn);
}

