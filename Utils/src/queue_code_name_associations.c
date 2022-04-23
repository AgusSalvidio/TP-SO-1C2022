#include <queue_code_name_associations.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <common_structures.h>
#include <stdlib.h>
#include <stdio.h>
#include <garbage_collector.h>
#include <general_logs.h>

t_dictionary* queue_code_name_associations;

void initialize_queue_code_name_associations(){

    queue_code_name_associations = dictionary_create();
    //dictionary_put(queue_code_name_associations, "OP1", (void*) operation_code);
}

char* queue_name_of(uint32_t queue_code){

    char* queue_name_to_find = NULL;

    void _find_queue_name(char* name, void* code){
        uint32_t cast_code = *((uint32_t*) code);
        if(queue_code == cast_code){
            queue_name_to_find = name;
        }
    }

    dictionary_iterator(queue_code_name_associations, _find_queue_name);

    if(!queue_name_to_find){
        log_queue_name_not_found_error(queue_code);
        free_system();
    }

    return queue_name_to_find;
}

uint32_t queue_code_of(char* queue_name){

    char* duplicated_queue_name = string_duplicate(queue_name);
    string_to_upper(duplicated_queue_name);

    uint32_t* queue_code_found = dictionary_get(queue_code_name_associations, duplicated_queue_name);

    if(!queue_code_found){
        free(duplicated_queue_name);
        log_queue_code_not_found_error(queue_name);
        free_system();
    }

    free(duplicated_queue_name);
    return *queue_code_found;
}

bool exists_queue_named(char* queue_name){
    char* duplicated_queue_name = string_duplicate(queue_name);
    string_to_upper(duplicated_queue_name);

    bool exists_queue = dictionary_has_key(queue_code_name_associations, duplicated_queue_name);

    free(duplicated_queue_name);

    return exists_queue;
}

void free_queue_code_name_associations(){
    dictionary_destroy_and_destroy_elements(queue_code_name_associations, free);
}
