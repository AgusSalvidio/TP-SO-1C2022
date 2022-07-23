#include <stdlib.h>
#include "kernel_cpu_connection.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/socket.h"
#include "kernel_configuration.h"
#include "kernel_cpu_message_handler.h"
#include "kernel_state_transitions.h"
#include "kernel_event.h"
#include "kernel_scheduler_queues.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_io_routine.h"

sem_t sem_preempt;

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
    t_pcb *returned_pcb;
    if (receive_information->receive_was_successful) {

        t_serialization_information *serialization_information = receive_information->serialization_information;
        t_request *deserialized_request = deserialize(serialization_information->serialized_request);

        t_message_handler *message_handler = message_handler_for_operation(deserialized_request->operation);
        returned_pcb = message_handler->perform_function(deserialized_request->structure, burst);

        pcb->page_table = returned_pcb->page_table;
        pcb->pc = returned_pcb->pc;

        free_serialization_information(serialization_information);
        free(deserialized_request);
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

    log_info(process_execution_logger(), "CPU Interrupted");

    free(ack);
    free_and_close_connection_information(connection_information);
    safe_sem_wait(&sem_preempt);
}

void initialize_cpu_structures() {
    safe_sem_initialize(&sem_preempt);
}

void free_cpu_structures() {
    safe_sem_destroy(&sem_preempt);
}