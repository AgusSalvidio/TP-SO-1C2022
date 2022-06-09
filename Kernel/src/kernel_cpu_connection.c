#include <stdlib.h>
#include "kernel_cpu_connection.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"

t_pcb * connect_and_send_pcb_to_cpu(void *structure_to_send) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = PCB;
    request->structure = structure_to_send;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_cpu_ip(), get_cpu_dispatch_port());

    serialize_and_send_structure(request, connection_information->socket_fd);

    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);
    t_pcb *pcb = structure_to_send;
    if(receive_information -> receive_was_successful){

        t_serialization_information* serialization_information = receive_information -> serialization_information;
        t_request* deserialized_request = deserialize(serialization_information -> serialized_request);
        log_request_received(process_execution_logger(), deserialized_request);
        pcb = (t_pcb*) deserialized_request -> structure;

        free_serialization_information(serialization_information);
        free(deserialized_request);
    }

    free(receive_information);

    stop_considering_garbage(request->structure);
    free(request);
    free_and_close_connection_information(connection_information);
    return pcb;
}

void connect_and_send_interruption_to_cpu() {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = INTERRUPTION;
    request->structure = NULL;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information;
    connection_information = connect_to(get_cpu_ip(), get_cpu_interrupt_port());


    serialize_and_send_structure_and_wait_for_ack(request, connection_information->socket_fd,
                                                  60000);

    stop_considering_garbage(request->structure);
    free_request(request);
    free_and_close_connection_information(connection_information);
}