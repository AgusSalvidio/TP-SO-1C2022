#include <commons/string.h>
#include <pretty_printer.h>
#include <stdlib.h>
#include <errno.h>
#include <queue_code_name_associations.h>
#include "../include/general_logs.h"
#include "logger.h"
#include "string.h"

void log_succesful_start_up(){
    char* message = string_new();
    string_append(&message, get_program_name());
    string_append(&message," se ha inicializado correctamente!\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_successful_execution(){
    char* message = string_new();
    string_append(&message, get_program_name());
    string_append(&message, " ha ejecutado exitosamente!\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_successful_clean_up(){
    char *message = string_new();
    string_append(&message, get_program_name());
    string_append(&message, " ha liberado toda la memoria ocupada exitosamente!\n");
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_initialize_pretty_printer(){
    log_succesful_message(process_execution_logger(), "Pretty printer se ha inicializado correctamente!\n");
}

void log_succesful_initialize_serializable_objects(){
    log_succesful_message(process_execution_logger(), "Los serializable_objects se han inicializado correctamente!\n");
}

void log_syscall_error_with_errno_description(char* message_error){
    char* message = string_from_format("%s: %s", message_error, strerror(errno));
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_syscall_error(char* message_error){
    log_errorful_message(process_execution_logger(), message_error);
}

void log_send_all_error(int sent_bytes, int amount_of_bytes){
    char* message = string_from_format("%s. Cantidad de bytes enviados: %d Total que debia enviarse: %d", "Error en send_all", sent_bytes, amount_of_bytes);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_pthread_create_error(unsigned int process_id){
    char* message = string_from_format("Ocurri?? un error al intentar crear un hilo para el proceso: %u", process_id);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_list_invalid_index_access(int index, int another_index,t_list* self){
    char *message = string_from_format("Hubo un error swapeando el indice i:'%d' con j:'%d' cuando el tama??o de lista es: '%u'",index,another_index,list_size(self));
    log_errorful_message(process_execution_logger(),message);
    free(message);
}

void log_queue_name_not_found_error(uint32_t queue_code){
    char* message = string_from_format("No se encontr?? una cola para el c??digo: %lu", queue_code);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_queue_code_not_found_error(char* queue_name){
    char* message = string_new();
    string_append(&message, "No se encontr?? una cola para el nombre: ");
    string_append(&message, queue_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_printable_object_not_found_error(){
    log_errorful_message(process_execution_logger(), "No se encontr?? un printable_object v??lido.");
}

void log_request_with_event(t_log* logger, t_request* request, char* event){
    char* message = string_new();
    string_append(&message, event);
    string_append(&message, ":\n");

    char* request_string = request_pretty_print(request);
    string_append(&message, request_string);
    log_succesful_message(logger, message);
    free(request_string);
    free(message);
}

void log_about_to_send_request(t_request* request){
    log_request_with_event(process_execution_logger(), request, "Pedido de enviar");
}

void log_request_sent(t_request* request){
    log_request_with_event(process_execution_logger(), request, "Pedido enviado");
}

void log_request_received_basic(){
    char* message = "Pedido recibido";
    log_succesful_message(process_execution_logger(), message);
    //log_succesful_message(main_logger(), message);
}

void log_request_received(t_log* logger, t_request* request){
    char* event = "Pedido recibido";
    log_request_with_event(logger, request, event);
}

void log_unknown_chained_evaluation_type_error(){
    log_errorful_message(process_execution_logger(), "Se ha configurado una chained_evaluation con un tipo inv??lido.");
}

void log_garbage_to_stop_considering_that_not_found_error(){
    log_errorful_message(process_execution_logger(), "Se esperaba encontrar un objeto designado como basura.");
}

void log_failed_ack_error(){
    log_errorful_message(process_execution_logger(), "Se recibi?? un ACK no esperado\n");
}

void log_succesful_suscription_to(t_log* logger, uint32_t operation_queue){
    char* message = string_from_format("Suscripci??n exitosa a la cola %s", queue_name_of(operation_queue));
    log_succesful_message(logger, message);
    free(message);
}

void log_errorful_not_existing_log(char* log_name){
    char* message = string_from_format("El log solicitado: '%s' no existe.",log_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_expected_to_have_only_one_element_error(){
    log_errorful_message(process_execution_logger(), "Se esperaba que la lista tuviera un ??nico elemento.");
}

void log_directory_could_not_open_in_path_error(char* path){
    char* message = string_from_format("No se pudo abrir un directorio en la ruta: %s", path);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_file_not_found_error(char* extension){
    char* message = string_from_format("No se encontr?? un archivo con extensi??n: %s", extension);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}