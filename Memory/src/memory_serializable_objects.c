#include <commons/collections/list.h>
#include <memory_serializable_objects.h>
#include <malloc.h>
#include "../../Utils/include/serializable_objects.h"

t_list* memory_operations_information;

void initialize_memory_serializable_objects(){

    initialize_serializable_objects();
    memory_operations_information = list_create();
    list_add(memory_operations_information,(void*) serializable_handshake());
    list_add(memory_operations_information,(void*) serializable_request_response());

}
void free_memory_serializable_objects(){

    //list_destroy_and_destroy_elements(memory_operations_information, free);
    list_destroy(memory_operations_information);
    free_serializable_objects();
}
