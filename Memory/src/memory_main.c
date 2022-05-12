#include <stdlib.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/serializable_objects.h"
#include <memory_logs_manager.h>
#include "../include/memory_file_management.h"
#include "memory_configuration_manager.h"
#include <memory_connection_handler.h>
#include <signal.h>
#include <memory_query_performers.h>
#include <memory_serializable_objects.h>
#include <memory_manager.h>
#include <string.h>


int main (void) {

    initialize_memory_configuration_manager();
    initialize_signal_handler();
    initialize_memory_file_management();
    initialize_garbage_collector();
    initialize_memory_logs_manager();
    initialize_memory_serializable_objects();
    initialize_memory_query_performers();

    log_succesful_start_up();

    execute_main_thread();

    //free_system();

    return EXIT_SUCCESS;
}





