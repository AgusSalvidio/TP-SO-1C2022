#include "../include/kernel_logs_manager.h"
#include "../../Utils/include/logger.h"
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

void log_pcb_new_to_ready_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d NEW -> READY.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_ready_to_exec_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d READY -> EXEC.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_blocked_to_ready_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d BLOCKED -> READY.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_blocked_to_suspended_blocked_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d BLOCKED -> SUSPENDED_BLOCKED.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_suspended_blocked_to_suspended_ready_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d SUSPENDED_BLOCKED -> SUSPENDED_READY.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_suspended_ready_to_ready_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d SUSPENDED_READY -> READY.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_exec_to_blocked_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d EXEC -> BLOCKED.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_exec_to_ready_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d EXEC -> READY.", pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_pcb_exec_to_exit_transition(uint32_t pid) {
    char *message = string_from_format("Proceso %d EXEC -> EXIT.", pid);
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

void free_kernel_logs_manager() {
    free_loggers();
}