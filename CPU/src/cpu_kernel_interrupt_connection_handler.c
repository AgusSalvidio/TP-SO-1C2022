#include <cpu_kernel_interrupt_connection_handler.h>
#include <unistring/stdint.h>
#include <stdlib.h>
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/socket.h"
#include "cpu_configuration_manager.h"
#include "cpu_query_performer.h"
#include "../../Utils/include/general_logs.h"


pthread_t interrupt_thread;
bool interruption_status = false;

bool current_interruption_status(){
    return interruption_status;
}

void modify_interruption_status(){
    interruption_status = false;
}

void interrupt_connection_handler(int server_socket_fd){

    int connection_fd = accept_incoming_connections_on(server_socket_fd);

    for ever{
        t_receive_information *receive_information = receive_structure(connection_fd);

        if (receive_information->receive_was_successful) {

            t_serialization_information* serialization_information = receive_information -> serialization_information;
            t_request* deserialized_request = deserialize(serialization_information -> serialized_request);
            log_request_received_basic();

            //t_query_performer* query_performer = query_performer_with_code(deserialized_request -> operation);
            interruption_status = true;

            send_ack_message(1, connection_fd);
            free_serialization_information(serialization_information);
        }
        free(receive_information);
    }
}

void initialize_cpu_interrupt_threads(){
    uint32_t interrupt_socket = listen_at(get_interrupt_port());
    interrupt_thread = default_safe_thread_create(interrupt_connection_handler, interrupt_socket);
}
