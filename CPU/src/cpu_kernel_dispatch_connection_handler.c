#include <cpu_kernel_dispatch_connection_handler.h>
#include <stdint.h>
#include <stdlib.h>
#include <cpu_configuration_manager.h>
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/general_logs.h"
#include "cpu_query_performer.h"

pthread_t dispatch_thread;


void dispatch_connection_handler(int server_socket_fd){
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
                free(resendable_request);
            }
            free_request(deserialized_request);
            free_serialization_information(serialization_information);
        }
        free(receive_information);
    }
}

void initialize_cpu_dispatch_threads(){
    uint32_t dispatch_socket = listen_at(get_dispatch_port());
    dispatch_thread = default_safe_thread_create(dispatch_connection_handler, dispatch_socket);
}

void free_dispatch_thread(){
    safe_thread_cancel(dispatch_thread);
}