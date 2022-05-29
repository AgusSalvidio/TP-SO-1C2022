#include <stdlib.h>
#include "console_kernel_connection.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/socket.h"
#include "console_configuration.h"
#include "../../Utils/include/garbage_collector.h"

void free_console_message_instructions(t_list *instructions) {
    void _destroy_operands(t_instruction* instruction) {
        list_destroy(instruction->operands);
        free(instruction);
    };

    list_destroy_and_destroy_elements(instructions, _destroy_operands);

}

void free_console(t_console_message* console_message) {
    free_console_message_instructions(console_message->instructions);
    free(console_message);
}

void connect_and_send_to_kernel(t_console_message * console_message) {

    t_request* request = safe_malloc(sizeof(t_request));
    request->operation = CONSOLE_MESSAGE;
    request->structure = console_message;
    request->sanitizer_function = free_console;

    char *message = pretty_print_of(request->operation, request->structure);
    log_info(process_execution_logger(), message);
    free(message);

    t_connection_information *connection_information = connect_to(get_kernel_ip(), get_kernel_port());

    serialize_and_send_structure_and_wait_for_ack(request, connection_information->socket_fd,
                                                  60000);

    stop_considering_garbage(request->structure);
    free_request(request);
    free_and_close_connection_information(connection_information);
}