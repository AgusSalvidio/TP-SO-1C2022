#include <cpu_serializable_objects.h>
#include <commons/collections/list.h>
#include "../../Utils/include/serializable_objects.h"

t_list* cpu_operations_information;

void initialize_cpu_serializable_objects(){

    initialize_serializable_objects();
    cpu_operations_information = list_create();
    list_add(cpu_operations_information,(void*) serializable_read());
    list_add(cpu_operations_information,(void*) serializable_write());
    list_add(cpu_operations_information,(void*) serializable_copy());
}
void free_cpu_serializable_objects(){

    list_destroy(cpu_operations_information);
    free_serializable_objects();
}
