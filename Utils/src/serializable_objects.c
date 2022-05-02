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

t_list* all_serializables;

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

void initialize_and_load_serializable_copy(){
    t_serializable_object* serializable_object = safe_malloc(sizeof(t_serializable_object));
    serializable_object -> code = COPY;
    serializable_object -> serialize_function = serialize_copy;
    serializable_object -> deserialize_function = deserialize_copy;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_serializable_objects(){

    initialize_queue_code_name_associations();

    all_serializables = list_create();

    initialize_and_load_serializable_read();
    initialize_and_load_serializable_write();
    initialize_and_load_serializable_copy();

    log_succesful_initialize_serializable_objects();
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