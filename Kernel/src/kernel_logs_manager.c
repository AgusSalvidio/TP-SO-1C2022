#include "../include/kernel_logs_manager.h"
#include "../../Utils/include/logger.h"

void initialize_kernel_logs_manager() {
    initialize_logger_for("Kernel");
    create_main_logger_from_config();
    create_process_execution_logger();
}

void free_kernel_logs_manager() {
    free_loggers();
}