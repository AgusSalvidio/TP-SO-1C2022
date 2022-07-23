#include <commons/string.h>
#include <malloc.h>
#include "kernel_fifo_algorithm.h"
#include "../../Utils/include/logger.h"
#include "kernel_logs_manager.h"

t_scheduling_algorithm *fifo_algorithm;

void fifo_resolve_dependencies_function() {}

void initialize_fifo_algorithm() {
    fifo_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    fifo_algorithm->algorithm_name = "FIFO";
    fifo_algorithm->resolve_dependencies_function = fifo_resolve_dependencies_function;
    fifo_algorithm->list_logger = log_ready_list;
}

t_scheduling_algorithm* get_fifo_algorithm() {
    return fifo_algorithm;
}
