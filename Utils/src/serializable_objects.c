#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/serializable_objects.h"
#include "../include/common_structures.h"
#include "../include/operation_serialization.h"
#include "../include/operation_deserialization.h"
#include <stdlib.h>
#include <t_list_extension.h>
#include <queue_code_name_associations.h>
#include <general_logs.h>
#include <request_bytes_calculator.h>

t_list* all_serializables;

void initialize_and_load_serializable_instruction() {
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = INSTRUCTION;
    serializable_object -> serialize_function = serialize_instruction;
    serializable_object -> deserialize_function = deserialize_instruction;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_console_message() {
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = CONSOLE_MESSAGE;
    serializable_object -> serialize_function = serialize_console_message;
    serializable_object -> deserialize_function = deserialize_console_message;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_handshake(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = HANDSHAKE;
    serializable_object -> serialize_function = serialize_handshake;
    serializable_object -> deserialize_function = deserialize_handshake;
  
    list_add(all_serializables, (void*) serializable_object);
}
void initialize_and_load_serializable_read(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = READ;
    serializable_object -> serialize_function = serialize_read;
    serializable_object -> deserialize_function = deserialize_read;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_write(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = WRITE;
    serializable_object -> serialize_function = serialize_write;
    serializable_object -> deserialize_function = deserialize_write;
    list_add(all_serializables, (void*) serializable_object);
}


void initialize_and_load_serializable_request_response(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = REQUEST_RESPONSE;
    serializable_object -> serialize_function = serialize_request_response;
    serializable_object -> deserialize_function = deserialize_request_response;
    list_add(all_serializables, (void*) serializable_object);
}
  
void initialize_and_load_serializable_copy(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = COPY;
    serializable_object -> serialize_function = serialize_copy;
    serializable_object -> deserialize_function = deserialize_copy;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_pcb() {
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = PCB;
    serializable_object -> serialize_function = serialize_pcb;
    serializable_object -> deserialize_function = deserialize_pcb;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_io_pcb(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = IO_PCB;
    serializable_object -> serialize_function = serialize_io_pcb;
    serializable_object -> deserialize_function = deserialize_io_pcb;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_first_access(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = FIRST_ACCESS;
    serializable_object -> serialize_function = serialize_first_access;
    serializable_object -> deserialize_function = deserialize_first_access;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_second_access(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = SECOND_ACCESS;
    serializable_object -> serialize_function = serialize_second_access;
    serializable_object -> deserialize_function = deserialize_second_access;
   list_add(all_serializables, (void*) serializable_object);
}
  
void initialize_and_load_serializable_initialize_process(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = INITIALIZE_PROCESS;
    serializable_object -> serialize_function = serialize_initialize_process;
    serializable_object -> deserialize_function = deserialize_initialize_process;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_suspend_process(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = SUSPEND_PROCESS;
    serializable_object -> serialize_function = serialize_suspend_process;
    serializable_object -> deserialize_function = deserialize_suspend_process;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_finalize_process(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = FINALIZE_PROCESS;
    serializable_object -> serialize_function = serialize_finalize_process;
    serializable_object -> deserialize_function = deserialize_finalize_process;
    list_add(all_serializables, (void*) serializable_object);
}

void initialize_serializable_objects(){

    initialize_queue_code_name_associations();

    all_serializables = list_create();

    //initialize all serializable objects and add to all_serializables
    initialize_and_load_serializable_instruction();
    initialize_and_load_serializable_console_message();
    initialize_and_load_serializable_handshake();
    initialize_and_load_serializable_request_response();
    initialize_and_load_serializable_read();
    initialize_and_load_serializable_write();
    initialize_and_load_serializable_copy();
    initialize_and_load_serializable_pcb();
    initialize_and_load_serializable_io_pcb();
    initialize_and_load_serializable_first_access();
    initialize_and_load_serializable_second_access();
    initialize_and_load_serializable_initialize_process();
    initialize_and_load_serializable_suspend_process();
    initialize_and_load_serializable_finalize_process();

  log_succesful_initialize_serializable_objects();
}


t_serializable_object* serializable_handshake(){
    return serializable_object_with_code(HANDSHAKE);
}
t_serializable_object* serializable_request_response(){
    return serializable_object_with_code(REQUEST_RESPONSE);
}  
t_serializable_object* serializable_read(){
    return serializable_object_with_code(READ);
}
t_serializable_object* serializable_write(){
    return serializable_object_with_code(WRITE);
}
t_serializable_object* serializable_copy(){
    return serializable_object_with_code(COPY);
}
t_serializable_object* serializable_first_access(){
    return serializable_object_with_code(FIRST_ACCESS);
}
t_serializable_object* serializable_second_access(){
    return serializable_object_with_code(SECOND_ACCESS);
}

t_serializable_object* serializable_object_with_code(uint32_t operation_code){

    bool _is_for(void* serializable_object){
        return ((t_serializable_object*) serializable_object) -> code == operation_code;
    }
    t_serializable_object* serializable_object_found = list_find(all_serializables, _is_for);

    return serializable_object_found;
}

void free_serializable_objects(){
    free_queue_code_name_associations();
    list_destroy_and_destroy_elements(all_serializables, free);
}

