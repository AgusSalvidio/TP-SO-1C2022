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

void initialize_serializable_objects(){

    initialize_queue_code_name_associations();

    all_serializables = list_create();

    //initialize all serializable objects and add to all_serializables

    log_succesful_initialize_serializable_objects();
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