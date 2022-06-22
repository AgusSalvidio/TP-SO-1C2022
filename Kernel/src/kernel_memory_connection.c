#include <stdlib.h>
#include "kernel_memory_connection.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "../../Utils/include/garbage_collector.h"

void connect_and_send_to_memory(uint32_t operation, void *structure_to_send) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = operation;
    request->structure = structure_to_send;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_memory_ip(), get_memory_port());

    serialize_and_send_structure_and_wait_for_ack(request, connection_information->socket_fd,
                                                  60000);

    stop_considering_garbage(request->structure);
    free_request(request);
    free_and_close_connection_information(connection_information);
}