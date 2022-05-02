#include <cpu_connection_handler.h>
#include <stdint.h>
#include <stdlib.h>
#include <cpu_configuration_manager.h>
#include <cpu_query_performer.h>
#include "../../Utils/include/socket.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/logger.h"

void connection_handler(int server_socket_fd){

    int connection_fd = accept_incoming_connections_on(server_socket_fd);

    for ever{

        t_receive_information *receive_information = receive_structure(connection_fd);

        if (receive_information->receive_was_successful) {

            t_serialization_information* serialization_information = receive_information -> serialization_information;
            t_request* deserialized_request = deserialize(serialization_information -> serialized_request);
            //Loguear y mostrar por consola mensaje recibido
            log_request_received(process_execution_logger(), deserialized_request);


            t_query_performer* query_performer = query_performer_with_code(deserialized_request -> operation);

            void* resendable_request = query_performer -> perform_function (deserialized_request);

            if(resendable_request){//if not null, send message back to whoever called us
                t_request* request_to_send = (t_request*) resendable_request;
                serialize_and_send_structure(request_to_send, connection_fd);
                free_request(resendable_request);
            }
            free_serialization_information(serialization_information);
            free_request(deserialized_request);
        }
        free(receive_information);
    }
}


void execute_main_thread(){
    //Conexión con memoria
    uint32_t memory_socket_fd = listen_at(get_memory_port());
    connection_handler(memory_socket_fd);

    //Conexión con kernel

}