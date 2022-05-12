#include <stdlib.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/general_logs.h"
#include "../include/kernel_logs_manager.h"

int main(void){

    initialize_configuration_manager();
    initialize_kernel_logs_manager();
    initialize_pretty_printer();
    initialize_garbage_collector();
    initialize_serializable_objects();
    initialize_signal_handler();

    log_succesful_start_up();
    //TODO: invocacion logica principal
    //
    free_system();
    return EXIT_SUCCESS;

}

