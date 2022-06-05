#include <malloc.h>
#include <commons/string.h>
#include "kernel_scheduling_algorithm.h"
#include "kernel_configuration.h"
#include "kernel_sanitizer.h"
#include "kernel_logs_manager.h"
#include "kernel_fifo_algorithm.h"
#include "kernel_srt_algorithm.h"

t_list* scheduling_algorithms;
t_scheduling_algorithm* scheduling_algorithm;
char *algorithm;

void initialize_scheduling_algorithms(){
    initialize_fifo_algorithm();
    initialize_srt_algorithm();

    scheduling_algorithms = list_create();
    list_add(scheduling_algorithms, get_fifo_algorithm());
    list_add(scheduling_algorithms, get_srt_algorithm());
}

bool can_handle(t_scheduling_algorithm* scheduling_algorithm){
    return string_equals_ignore_case(scheduling_algorithm -> algorithm_name, algorithm);
}

void free_scheduling_algorithms(){
    list_destroy_and_destroy_elements(scheduling_algorithms, free);
}

t_scheduling_algorithm* chosen_scheduling_algorithm(){
    algorithm = get_scheduling_algorithm();

    initialize_scheduling_algorithms();

    t_scheduling_algorithm* scheduling_algorithm_found =
            list_remove_by_condition(scheduling_algorithms, (bool (*)(void *)) can_handle);

    if(!scheduling_algorithm_found){
        log_scheduling_algorithm_not_found_error_for(algorithm);
        free_system();
    }

    free_scheduling_algorithms();

    return scheduling_algorithm_found;
}

void initialize_scheduling_algorithm() {
    scheduling_algorithm = chosen_scheduling_algorithm();
    scheduling_algorithm -> resolve_dependencies_function();
}

void free_scheduling_algorithm() {
    free(scheduling_algorithm);
}