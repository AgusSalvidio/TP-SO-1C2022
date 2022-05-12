#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/operation_deserialization.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/configuration_manager.h"
#include <memory_configuration_manager.h>
#include <memory_logs_manager.h>
#include <memory_query_performers.h>
#include <commons/string.h>

void connection_handler(int server_socket_fd){

    for ever{

        int connection_fd = accept_incoming_connections_on(server_socket_fd);

        t_receive_information *receive_information = receive_structure(connection_fd);

        if (receive_information->receive_was_successful) {

            t_serialization_information* serialization_information = receive_information -> serialization_information;
            t_request* deserialized_request = deserialize(serialization_information -> serialized_request);
            log_request_received_basic();

            t_query_performer* query_performer = query_performer_with_code(deserialized_request -> operation);

            void* resendable_request = query_performer -> perform_function (deserialized_request);

            if(resendable_request){//if not null, send message back to whoever called us
                t_request* request_to_send = (t_request*) resendable_request;
                serialize_and_send_structure(request_to_send, connection_fd);
                free_request(resendable_request);
            }
            free_serialization_information(serialization_information);
        }
        free(receive_information);
    }
}

void execute_main_thread(){
    int server_socket_fd = listen_at(port_getter());
    connection_handler(server_socket_fd);
}
