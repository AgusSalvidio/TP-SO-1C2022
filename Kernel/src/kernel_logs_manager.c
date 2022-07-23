#include "../include/kernel_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/common_structures.h"
#include <stdint.h>
#include <commons/string.h>
#include <malloc.h>

void initialize_kernel_logs_manager() {
    initialize_logger_for("Kernel");
    create_main_logger_from_config();
    create_process_execution_logger();
}

void log_invalid_transition_error() {
    log_errorful_message(process_execution_logger(), "La transicion de estado es invalida.");
}

void log_pcb_added_to_new_queue(uint32_t pid) {
    char *message = string_from_format("Proceso %d -> NEW.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_new_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max) {
    char *message = string_from_format("Proceso %d NEW -> READY. Cantidad de procesos actual %d / Cantidad de espacios disponibles %d", pid, max - available, available);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_ready_to_exec_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d READY -> EXEC.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_blocked_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max) {
    char *message = string_from_format("Proceso %d BLOCKED -> READY. Cantidad de procesos actual %d / Cantidad de espacios disponibles %d", pid, max - available, available);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_blocked_to_suspended_blocked_transition(uint32_t pid, uint32_t available, uint32_t max) {
    char *message = string_from_format("Proceso %d BLOCKED -> SUSPENDED_BLOCKED. Cantidad de procesos actual %d / Cantidad de espacios disponibles %d", pid, max-available-1, available+1);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_suspended_blocked_to_suspended_ready_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d SUSPENDED_BLOCKED -> SUSPENDED_READY.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_suspended_ready_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max) {
    char *message = string_from_format("Proceso %d SUSPENDED_READY -> READY. Cantidad de procesos actual %d / Cantidad de espacios disponibles %d", pid, max - available, available);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_exec_to_blocked_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d EXEC -> BLOCKED.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_exec_to_ready_transition(uint32_t pid, uint32_t available, uint32_t max) {
    char *message = string_from_format("Proceso %d EXEC -> READY. Cantidad de procesos actual %d / Cantidad de espacios disponibles %d", pid, max - available, available);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_exec_to_exit_transition(uint32_t pid, uint32_t available, uint32_t max) {
    char *message = string_from_format("Proceso %d EXEC -> EXIT. Cantidad de procesos actual %d / Cantidad de espacios disponibles %d", pid, max - available - 1, available +1);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_blocked_to_exit_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d BLOCKED -> EXIT.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_suspended_blocked_to_exit_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d SUSPENDED_BLOCKED -> EXIT.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_invalid_state_error() {
    log_errorful_message(process_execution_logger(), "La cola es inválida.");
}

void log_pcb_not_found_error(uint32_t pid) {
    char *error_message = string_from_format("El pcb con pid %d no fue encontrado.", pid);
    log_errorful_message(process_execution_logger(), error_message);
    free(error_message);
}

void log_scheduling_algorithm_not_found_error_for(char *algorithm_name) {
    char *message = string_from_format("No se ha implementado un algoritmo de planificación llamado %s",
                                       algorithm_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_context_executing(uint32_t pid) {
    char *message = string_from_format("Pcb %d ejecutando.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_io_starting_execution(uint32_t pid, uint32_t millis) {
    char *message = string_from_format("Comienza ejecucion de IO por proceso %d... duracion: %dms", pid,
                                       millis);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_io_finished_execution(uint32_t pid) {
    char *message = string_from_format("Finaliza ejecucion de IO por proceso %d", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_estimation_list(t_list* list_of_pcbs) {
    char* to_log = string_new();
    void _print_estimation(t_pcb* pcb) {
        char *to_append = string_from_format("(pid: %d, est: %f),", pcb->pid, pcb->next_burst);
        string_append(&to_log, to_append);
        free(to_append);
    };

    list_iterate(list_of_pcbs, (void (*)(t_pcb *))_print_estimation);
    log_debug(process_execution_logger(), to_log);
    free(to_log);
}

void log_ready_list(t_list* pcb_list) {
    char* to_log = string_new();
    void _print_pid(t_pcb* pcb) {
        char *to_append = string_from_format("(pid: %d),", pcb->pid);
        string_append(&to_log, to_append);
        free(to_append);
    };

    list_iterate(pcb_list, (void (*)(t_pcb *))_print_pid);
    log_debug(process_execution_logger(), to_log);
    free(to_log);
}

void log_update_current_estimation(t_pcb* pcb, double previous, double actual) {
    char *message = string_from_format("Actualizacion de estimacion PID %d ; %f --> %f", pcb->pid, previous, actual);
    log_debug(process_execution_logger(), message);
    free(message);
}

void log_calculate_estimation(t_pcb* pcb, double est, double real) {
    char *message = string_from_format("Calculo de estimacion PID %d ; Estimada Anterior = %f ; Real Anterior = %f ; Siguiente = %f", pcb->pid, est, real, pcb->next_burst);
    log_debug(process_execution_logger(), message);
    free(message);
}

void free_kernel_logs_manager() {
    free_loggers();
}