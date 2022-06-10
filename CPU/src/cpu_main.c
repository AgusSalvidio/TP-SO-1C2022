#include <stdlib.h>
#include <cpu_configuration_manager.h>
#include <cpu_logs_manager.h>
#include <cpu_serializable_objects.h>
#include <cpu_query_performer.h>
#include <cpu_memory_connection_handler.h>
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "cpu_action_performer.h"
#include <cpu_kernel_dispatch_connection_handler.h>


int main(void){

    initialize_signal_handler();
    initialize_cpu_configuration_manager();
    //initialize_cpu_file_management();
    initialize_garbage_collector();
    initialize_cpu_logs_manager();
    initialize_cpu_serializable_objects();
    initialize_cpu_query_performers();
    initialize_action_performers();

    log_succesful_start_up();

    //send_handshake_to_memory();
    //initialize_mmu();
    //initialize_cpu_dispatch_threads();
    //initialize_cpu_interrupt_threads();


    return EXIT_SUCCESS;
}

