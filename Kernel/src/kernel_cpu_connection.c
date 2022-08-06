#include <stdlib.h>
#include "kernel_cpu_connection.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "kernel_cpu_message_handler.h"
#include "kernel_scheduler_queues.h"

sem_t sem_preempt;
bool interrupted;

t_burst *connect_and_send_pcb_to_cpu(t_pcb *pcb) {

    t_request *request = safe_malloc(sizeof(t_request));
    request->operation = PCB;
    request->structure = pcb;
    request->sanitizer_function = free;

    t_connection_information *connection_information = connect_to(get_cpu_ip(), get_cpu_dispatch_port());
    t_burst *burst = safe_malloc(sizeof(t_burst));
    burst->pcb = pcb;
    burst->start = current_time_in_milliseconds();
    serialize_and_send_structure(request, connection_information->socket_fd);
    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);
    burst->finished = current_time_in_milliseconds();
    if (receive_information->receive_was_successful) {

        t_serialization_information *serialization_information = receive_information->serialization_information;
        t_request *deserialized_request = deserialize(serialization_information->serialized_request);
        t_message_handler *message_handler = message_handler_for_operation(deserialized_request->operation);
        message_handler->perform_function(deserialized_request->structure, burst);

        free_serialization_information(serialization_information);
        free_request(deserialized_request);
    }

    free(receive_information);

    free(request);
    free_and_close_connection_information(connection_information);
    return burst;
}

void connect_and_send_interruption_to_cpu() {

    t_connection_information *connection_information;
    connection_information = connect_to(get_cpu_ip(), get_cpu_interrupt_port());

    send_ack_message(1, connection_information->socket_fd);
    void *ack = receive_ack_with_timeout_in_seconds(connection_information->socket_fd, 60000);

    free(ack);
    free_and_close_connection_information(connection_information);
    log_info(process_execution_logger(), "CPU Interrupted");
    if (!list_is_empty(scheduler_queue_of(EXEC)->pcb_list))
        interrupted = true;
        safe_sem_wait(&sem_preempt);
}

void initialize_cpu_structures() {
    safe_sem_initialize(&sem_preempt);
    interrupted = false;
}

void free_cpu_structures() {
    safe_sem_destroy(&sem_preempt);
}