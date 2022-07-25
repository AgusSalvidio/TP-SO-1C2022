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

void log_swap_file_delete_procedure_description(char* swap_file_path, char* description){

    char* message = string_from_format("El archivo ubicado en %s %s\n",swap_file_path,description);
    log_info(process_execution_logger(), message);
    free(message);

}

void log_main_memory_was_successfully_initialized(){
    char* message = string_from_format("La memoria principal fue inicializada exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_memory_table_manager_was_successfully_initialized(){
    char* message = string_from_format("El administrador de tablas de memoria fue inicializado exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_process_context_manager_was_successfully_initialized(){
    char* message = string_from_format("El administrador de contextos de procesos fue inicializado exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_first_level_table_for_process_was_successfully_initialized(uint32_t pid){

    char* message = string_from_format("La tabla de primer nivel para el proceso: %d fue inicializada exitosamente.\n",pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_second_level_table_for_process_was_successfully_initialized(uint32_t pid){

    char* message = string_from_format("La tabla de segundo nivel para el proceso: %d fue inicializada exitosamente.\n",pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_swap_file_for_process_was_successfully_initialized(uint32_t pid,char* swap_file_path){

    char* message = string_from_format("El archivo para el proceso %d ubicado en %s fue creado exitosamente.\n",pid,swap_file_path);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_process_context_was_successfully_initialized(uint32_t pid){
    char* message = string_from_format("El contexto de procesos para el proceso: %d fue inicializado exitosamente.\n",pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_cannot_initialize_new_process_because(char* description){
    log_errorful_message(process_execution_logger(), description);
    free(description);

}

void log_main_memory_frames_were_successfully_released(){
    char* message = string_from_format("Los frames de memoria fueron liberados exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_process_operation_was_succesful_for(uint32_t pid,char* operation_state){
    char* message = string_from_format("El proceso %d fue %s exitosamente.\n",pid,operation_state);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_process_suspension_was_successful(uint32_t pid){
    log_process_operation_was_succesful_for(pid,"suspendido");
}

void log_process_finalization_was_successful(uint32_t pid){
    log_process_operation_was_succesful_for(pid,"finalizado");
}
void log_handshake_was_sent_succesfully(){
    char* message = string_from_format("El Handshake con las configuraciones iniciales fue enviado exitosamente.\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_request_was_handled_succesfully_for(char* request_type){
    char* message = string_from_format("El pedido de %s fue handleado exitosamente.\n",request_type);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_write_request_was_handled_successfully(){
    log_request_was_handled_succesfully_for("escritura");
}
void log_read_request_was_handled_successfully(){
    log_request_was_handled_succesfully_for("lectura");
}

void log_cpu_first_access_was_handled_successfully(uint32_t index,uint32_t entry){
    char* message = string_from_format("El primer acceso de CPU a memoria para el indice de tabla de primer nivel: %d en la entrada: %d fue handleado exitosamente.\n",index,entry);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_cpu_second_access_was_handled_successfully(uint32_t index,uint32_t entry){
    char* message = string_from_format("El segundo acceso de CPU a memoria para el indice de tabla de segundo nivel: %d en la entrada: %d fue handleado exitosamente.\n",index,entry);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}
void log_cpu_second_access_cannot_be_handled(uint32_t index,uint32_t entry){

    char* message = string_from_format("El segundo acceso de CPU a memoria para el indice de tabla de segundo nivel: %d en la entrada: %d no pudo ser handleado.\n",index,entry);
    log_errorful_message(process_execution_logger(), message);
    free(message);

}

void log_memory_read_at(uint32_t frame,uint32_t offset){
    char* message = string_from_format("Se produjo una lectura en memoria principal en el frame: %d con desplazamiento: %d.\n",frame,offset);
    log_info(process_execution_logger(), message);
    free(message);

}

void log_memory_write_at(uint32_t frame,uint32_t offset, uint32_t value_to_write){
    char* message = string_from_format("Se produjo una escritura en memoria principal en el frame: %d con desplazamiento: %d con el valor: %d.\n",frame,offset,value_to_write);
    log_info(process_execution_logger(), message);
    free(message);

}

void log_page_was_loaded_in_memory_successfully(uint32_t pid, t_page* page){
    char* message = string_from_format("La carga de la pagina %d para el proceso %d fue compleada exitosamente.\n",page->id,pid);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_frame_related_to_page_was_added_to_process_context_successfully(uint32_t frame, uint32_t page_id){
    char* message = string_from_format("Se agrego exitosamente al contexto de proceso la relacion del frame %d asociado con la pagina %d.\n",frame,page_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);

}

void log_swap_procedure_was_successful(uint32_t pid,uint32_t victim_page_id,uint32_t selected_page_id){

    char* message = string_from_format("Se realizo el swap del proceso %d exitosamente. Se swapeo la pagina %d por %d.\n",pid,victim_page_id,selected_page_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void free_memory_logs_manager(){
    free_loggers();
}