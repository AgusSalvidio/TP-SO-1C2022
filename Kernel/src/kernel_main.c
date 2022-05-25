#include <stdlib.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/general_logs.h"
#include "../include/kernel_logs_manager.h"
#include "kernel_console_connection.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"

int main(void) {

    initialize_configuration_manager();
    initialize_kernel_logs_manager();
    initialize_pretty_printer();
    initialize_garbage_collector();
    initialize_serializable_objects();
    initialize_signal_handler();

    initialize_scheduler_queues();
    initialize_state_transitions();
    initialize_kernel_process_image();

    log_succesful_start_up();
    //TODO: invocacion logica principal

    execute_connection_handler();
    //
    free_system();
    return EXIT_SUCCESS;

}

