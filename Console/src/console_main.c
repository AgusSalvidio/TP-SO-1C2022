#include <stdlib.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/general_logs.h"
#include "console_logs_manager.h"
#include "console_args_parser.h"
#include "console_kernel_connection.h"
#include "../../Utils/include/logger.h"
#include "console_instruction_handler.h"

int main(int argc, char** argv){
    if(argc < 3) {
        return EXIT_FAILURE;
    }

    initialize_configuration_manager();
    initialize_console_logs_manager();
    initialize_pretty_printer();
    initialize_garbage_collector();
    initialize_serializable_objects();
    initialize_signal_handler();
    initialize_instruction_creator();

    log_succesful_start_up();
    //TODO: invocacion logica principal
    t_console_message* console_message = parse_program_args(argv);

    connect_and_send_to_kernel(console_message);

    //
    free_system();
    return EXIT_SUCCESS;

}

