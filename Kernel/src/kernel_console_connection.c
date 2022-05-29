#include "../include/kernel_console_connection.h"
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <signal.h>
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"
#include "kernel_console_message_handler.h"
#include "../../Utils/include/logger.h"

void* connection_controller(void* socket_fd){

    int client_socket_fd = *(int*) socket_fd;

    t_receive_information* receive_information = receive_structure(client_socket_fd);

    if(receive_information -> receive_was_successful){

        t_serialization_information* serialization_information = receive_information -> serialization_information;
        t_request* deserialized_request = deserialize(serialization_information -> serialized_request);
        log_request_received(process_execution_logger(), deserialized_request);
        handle_console_message(deserialized_request -> structure);
        send_ack_message(1, client_socket_fd);

        free_serialization_information(serialization_information);
        free(deserialized_request);
    }

    free(receive_information);
    free(socket_fd);

    return NULL;
}

void commit_suicide(){
    pthread_exit(NULL);
}

void execute_connection_handler() {
    handle_signal(SIGUSR1, commit_suicide);
    start_multithreaded_server(get_listening_port(), connection_controller);
}