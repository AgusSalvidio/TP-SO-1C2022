#include "../../Utils/include/logger.h"
#include "../include/memory_logs_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/common_structures.h"
#include <commons/string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory_manager.h>

void initialize_memory_logs_manager(){

    initialize_logger_for("memory");
    create_main_logger_from_config();
    create_process_execution_logger();

}


void log_memory_query_performers_loaded_succesfully(){

    char* message = string_from_format("Los query performers fueron cargados exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}
void log_query_performer_not_found_error_from_memory(uint32_t operation){

    char* message = string_from_format("No se ha encontrado un query performer que maneje operaciones de código %u\n", operation);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}


void free_memory_logs_manager(){

    free_loggers();
}