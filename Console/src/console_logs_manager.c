#include "console_logs_manager.h"
#include "../../Utils/include/logger.h"
#include <commons/string.h>
#include <stdlib.h>

void initialize_console_logs_manager() {
    initialize_logger_for("Console");
    create_main_logger_from_config();
    create_process_execution_logger();
}

void free_console_logs_manager() {
    free_loggers();
}

void log_invalid_transition_error() {
    log_errorful_message(process_execution_logger(), "La instruccion es inexistente.");
}

void log_invalid_number_of_params(char *code, uint32_t expected, uint32_t actual) {
    char *message = string_from_format(
            "Incorrect number of params for instruction %s. Expected: %d -> Actual: %d.", code, expected, actual);
    log_errorful_message(process_execution_logger(), message);
    free(message);

}