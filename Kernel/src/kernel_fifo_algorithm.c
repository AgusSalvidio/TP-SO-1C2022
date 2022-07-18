#include "kernel_fifo_algorithm.h"

t_scheduling_algorithm *fifo_algorithm;

void fifo_resolve_dependencies_function() {}

void initialize_fifo_algorithm() {
    fifo_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    fifo_algorithm->algorithm_name = "FIFO";
    fifo_algorithm->resolve_dependencies_function = fifo_resolve_dependencies_function;
}

t_scheduling_algorithm* get_fifo_algorithm() {
    return fifo_algorithm;
}
