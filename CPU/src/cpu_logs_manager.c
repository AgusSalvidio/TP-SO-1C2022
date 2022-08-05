#include <cpu_logs_manager.h>
#include <commons/string.h>
#include "../../Utils/include/logger.h"
#include <stdlib.h>
#include <stdint.h>

void initialize_cpu_logs_manager(){
    initialize_logger_for("cpu");
    create_main_logger_from_config();
    create_process_execution_logger();
}

void log_read_content(uint32_t content){
    char* message = string_from_format("El contenido leido es: %d.\n", content);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}
void log_cpu_query_performers_loaded_succesfully(){
    char* message = string_from_format("Los query performers fueron cargados exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}
void log_query_performer_not_found_error_from_cpu(uint32_t operation){
    char* message = string_from_format("No se ha encontrado un query performer que maneje operaciones de código %u\n", operation);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}
void log_cpu_action_performers_loaded_succesfully(){
    char* message = string_from_format("Los action performers fueron cargados exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}
void log_handshake_received_succesfully(){
    char* message = string_from_format("El handshake fue recibido correctamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}
void log_tlb_succesfully_created(){
    char* message = string_from_format("La TLB fue creada correctamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_current_instruction_running(char* instruction){
    char* message = string_from_format("Se esta ejecutando la operación %s.\n", instruction);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_return_pcb_to_kernel(){
    char* message = string_from_format("Se devuelve el PCB a Kernel.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_tlb_hit(){
    char* message = string_from_format("La página se encuentra en la TLB.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void free_cpu_logs_manager(){
    free_loggers();
}