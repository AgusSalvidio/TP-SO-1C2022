#include <stdlib.h>
#include <commons/string.h>
#include "kernel_memory_connection.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"

void free_request_response(t_request_response* request_response) {
    free(request_response->type_description);
    stop_considering_garbage(request_response);
    free(request_response);
}

uint32_t connect_and_send_initialize_to_memory(void *structure_to_send) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = INITIALIZE_PROCESS;
    request->structure = structure_to_send;
    request->sanitizer_function = free;

    t_connection_information *connection_information = connect_to(get_memory_ip(), get_memory_port());

    serialize_and_send_structure(request, connection_information->socket_fd);
    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);
    uint32_t page_table_id = 0;
    if(receive_information -> receive_was_successful){

        t_serialization_information* serialization_information = receive_information -> serialization_information;
        t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

        t_request_response *request_response = (t_request_response*) deserialized_request->structure;
        page_table_id = request_response->content;
        if (string_equals_ignore_case(request_response->type_description, "ERROR")) {
            free_system();
        }

        free_request_response(request_response);
        free_serialization_information(serialization_information);
        free(deserialized_request);
    }

    free(receive_information);
    free_request(request);
    free_and_close_connection_information(connection_information);
    return page_table_id;
}

void connect_and_send_suspend_to_memory(void *structure_to_send) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = SUSPEND_PROCESS;
    request->structure = structure_to_send;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_memory_ip(), get_memory_port());

    serialize_and_send_structure(request, connection_information->socket_fd);
    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);
    if(receive_information -> receive_was_successful){

        t_serialization_information* serialization_information = receive_information -> serialization_information;
        t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

        t_request_response *request_response = (t_request_response*) deserialized_request->structure;
        if (string_equals_ignore_case(request_response->type_description, "ERROR")) {
            free_system();
        }
        free_request_response(request_response);
        free_serialization_information(serialization_information);
        free(deserialized_request);
    }

    free(receive_information);
    free_request(request);
    free_and_close_connection_information(connection_information);
}

void connect_and_send_finalize_to_memory(void *structure_to_send) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = FINALIZE_PROCESS;
    request->structure = structure_to_send;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_memory_ip(), get_memory_port());

    serialize_and_send_structure(request, connection_information->socket_fd);
    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);
    if(receive_information -> receive_was_successful){

        t_serialization_information* serialization_information = receive_information -> serialization_information;
        t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

        t_request_response *request_response = (t_request_response*) deserialized_request->structure;
        if (string_equals_ignore_case(request_response->type_description, "ERROR")) {
            free_system();
        }
        free_request_response(request_response);
        free_serialization_information(serialization_information);
        free(deserialized_request);
    }

    free(receive_information);
    free_request(request);
    free_and_close_connection_information(connection_information);
}