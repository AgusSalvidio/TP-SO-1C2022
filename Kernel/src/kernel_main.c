#include <stdlib.h>
#include "../include/planification.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/general_logs.h"
#include "../include/kernel_logs_manager.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "kernel_event.h"
#include "kernel_io_routine.h"
#include "kernel_cpu_message_handler.h"

int main(void) {

    initialize_configuration_manager();
    initialize_kernel_logs_manager();
    initialize_pretty_printer();
    initialize_garbage_collector();
    initialize_serializable_objects();
    initialize_signal_handler();
    initialize_event_notifier();

    initialize_scheduler_queues();
    initialize_state_transitions();
    initialize_io_routine();
    initialize_kernel_process_image();
    initialize_cpu_message_handler();

    log_succesful_start_up();

    iniciar_planificador_corto_plazo();
    iniciar_planificador_mediano_plazo();
    iniciar_planificador_largo_plazo();

    free_system();
    return EXIT_SUCCESS;
}

