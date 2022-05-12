#include <stdlib.h>
#include "console_kernel_connection.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/socket.h"
#include "console_configuration.h"
#include "../../Utils/include/garbage_collector.h"

void connect_and_send_to_kernel(t_console_message * console_message) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = CONSOLE_MESSAGE;
    request->structure = console_message;
    request->sanitizer_function = free;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_kernel_ip(), get_kernel_port());

    serialize_and_send_structure_and_wait_for_ack(request, connection_information->socket_fd,
                                                  60000);

    free_request(request);
    free_and_close_connection_information(connection_information);
}