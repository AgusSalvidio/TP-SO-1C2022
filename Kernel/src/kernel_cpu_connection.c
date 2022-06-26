#include <stdlib.h>
#include <commons/string.h>
#include "kernel_cpu_connection.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"
#include "kernel_cpu_message_handler.h"

t_burst * connect_and_send_pcb_to_cpu(t_pcb *pcb) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = PCB;
    request->structure = pcb;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_cpu_ip(), get_cpu_dispatch_port());
    t_burst *burst = safe_malloc(sizeof(t_burst));
    burst->pcb = pcb;
    burst->start = current_time_in_milliseconds();
    serialize_and_send_structure(request, connection_information->socket_fd);
    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);
    burst->finished = current_time_in_milliseconds();
    t_pcb *returned_pcb;
    if(receive_information -> receive_was_successful){

        t_serialization_information* serialization_information = receive_information -> serialization_information;
        t_request* deserialized_request = deserialize(serialization_information -> serialized_request);
        log_request_received(process_execution_logger(), deserialized_request);

        t_message_handler * message_handler = message_handler_for_operation(deserialized_request -> operation);
        returned_pcb = message_handler -> perform_function (deserialized_request -> structure);

        pcb->page_table = returned_pcb->page_table;
        pcb->pc = returned_pcb->pc;

        free_serialization_information(serialization_information);
        free(deserialized_request);
    }

    free(receive_information);

    stop_considering_garbage(request->structure);
    free(request);
    free_and_close_connection_information(connection_information);
    return burst;
}

void connect_and_send_interruption_to_cpu() {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = INTERRUPTION;
    request->structure = NULL;
    request->sanitizer_function = free;

    log_info(process_execution_logger(), "Sent Interruption to CPU");

    t_connection_information *connection_information;
    connection_information = connect_to(get_cpu_ip(), get_cpu_interrupt_port());


    serialize_and_send_structure_and_wait_for_ack(request, connection_information->socket_fd,
                                                  60000);

    stop_considering_garbage(request->structure);
    free_request(request);
    free_and_close_connection_information(connection_information);
}