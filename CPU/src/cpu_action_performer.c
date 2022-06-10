#include <cpu_action_performer.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include "../../Utils/include/common_structures.h"
#include "cpu_manager.h"

t_list* action_performers;

void initialize_and_load_read_action_performer(){
    t_action_performer* action_performer = safe_malloc(sizeof(t_action_performer));
    action_performer -> operation = READ;
    action_performer -> perform_function = handle_read_request_procedure;

    list_add(action_performers, action_performer);
}

void initialize_and_load_write_action_performer(){
    t_action_performer* action_performer = safe_malloc(sizeof(t_action_performer));
    action_performer -> operation = WRITE;
    action_performer -> perform_function = handle_write_request_procedure;

    list_add(action_performers, action_performer);
}

void initialize_and_load_copy_action_performer(){
    t_action_performer* action_performer = safe_malloc(sizeof(t_action_performer));
    action_performer -> operation = COPY;
    action_performer -> perform_function = handle_copy_request_procedure;

    list_add(action_performers, action_performer);
}

void initialize_and_load_no_op_action_performer(){
    t_action_performer* action_performer = safe_malloc(sizeof(t_action_performer));
    action_performer -> operation = NO_OP;
    action_performer -> perform_function = handle_no_op_request_procedure;

    list_add(action_performers, action_performer);
}

void initialize_and_load_IO_action_performer(){
    t_action_performer* action_performer = safe_malloc(sizeof(t_action_performer));
    action_performer -> operation = IO;
    action_performer -> perform_function = handle_IO_request_procedure;

    list_add(action_performers, action_performer);
}

void initialize_and_load_exit_action_performer(){
    t_action_performer* action_performer = safe_malloc(sizeof(t_action_performer));
    action_performer -> operation = EXIT;
    action_performer -> perform_function = handle_exit_request_procedure;

    list_add(action_performers, action_performer);
}

void initialize_action_performers(){

    action_performers = list_create();

    initialize_and_load_read_action_performer();
    initialize_and_load_write_action_performer();
    initialize_and_load_copy_action_performer();
    initialize_and_load_no_op_action_performer();
    initialize_and_load_IO_action_performer();
    initialize_and_load_exit_action_performer();
}

t_action_performer* action_performer_with_code(uint32_t operation_code){

    bool _equals_operation(void* action_performer){
        return ((t_action_performer*) action_performer) -> operation == operation_code;
    }

    return list_find(action_performers, _equals_operation);
}

void free_action_performers(){
    list_destroy_and_destroy_elements(action_performers, free);
}