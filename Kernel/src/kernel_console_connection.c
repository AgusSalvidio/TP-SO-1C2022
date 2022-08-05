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
#include "../../Utils/include/pthread_wrapper.h"

void connection_controller(int socket_fd) {

    for ever {
        int connection_fd = accept_incoming_connections_on(socket_fd);

        t_receive_information *receive_information = receive_structure(connection_fd);

        if (receive_information->receive_was_successful) {

            t_serialization_information *serialization_information = receive_information->serialization_information;
            t_request *deserialized_request = deserialize(serialization_information->serialized_request);
            handle_console_message(deserialized_request->structure);
            send_ack_message(1, connection_fd);

            free_serialization_information(serialization_information);
            free(deserialized_request);
        }

        free(receive_information);
    }
}
