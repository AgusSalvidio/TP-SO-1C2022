#include <commons/collections/list.h>
#include <stdlib.h>
#include "console_instruction_handler.h"
#include "../../Utils/include/common_structures.h"
#include "console_sanitizer.h"
#include "console_logs_manager.h"
#include <commons/string.h>

t_list* instruction_creators;

t_list *create_no_op_instruction (char** instruction_array) {
    t_list *instructions = list_create();
    if (string_array_size(instruction_array) == 2) {
        for (int i = 1; i <= atoi(instruction_array[1]) ; ++i) {
            t_instruction *instruction = safe_malloc(sizeof (t_instruction));
            instruction -> type = NO_OP;
            instruction -> operands = list_create();
            list_add(instructions, instruction);
        }
        return instructions;
    } else {
        log_invalid_number_of_params(instruction_array[0], 0, string_array_size(instruction_array));
        free_system();
    }
}

t_list *create_io_instruction (char** instruction_array) {
    t_list *instructions = list_create();
    if (string_array_size(instruction_array) == 2) {
        t_instruction *instruction = safe_malloc(sizeof (t_instruction));
        instruction -> type = IO;
        t_list *operands = list_create();
        for (int i = 1; instruction_array[i] ; ++i) {
            list_add(operands, atoi(instruction_array[i]));
        }
        instruction -> operands = operands;
        list_add(instructions, instruction);
        return instructions;
    } else {
        log_invalid_number_of_params(instruction_array[0], 1, string_array_size(instruction_array) - 1);
        free_system();
    }
}

t_list *create_write_instruction (char** instruction_array) {
    t_list *instructions = list_create();
    if (string_array_size(instruction_array) == 3) {
        t_instruction *instruction = safe_malloc(sizeof (t_instruction));
        instruction -> type = WRITE;
        t_list *operands = list_create();
        for (int i = 1; instruction_array[i] ; ++i) {
            list_add(operands, atoi(instruction_array[i]));
        }
        instruction -> operands = operands;
        list_add(instructions, instruction);
        return instructions;
    } else {
        log_invalid_number_of_params(instruction_array[0], 2, string_array_size(instruction_array) - 1);
        free_system();
    }
}

t_list *create_read_instruction (char** instruction_array) {
    t_list *instructions = list_create();
    if (string_array_size(instruction_array) == 2) {
        t_instruction *instruction = safe_malloc(sizeof (t_instruction));
        instruction -> type = READ;
        t_list *operands = list_create();
        for (int i = 1; instruction_array[i] ; ++i) {
            list_add(operands, atoi(instruction_array[i]));
        }
        instruction -> operands = operands;
        list_add(instructions, instruction);
        return instructions;
    } else {
        log_invalid_number_of_params(instruction_array[0], 1, string_array_size(instruction_array) - 1);
        free_system();
    }
}

t_list *create_copy_instruction (char** instruction_array) {
    t_list *instructions = list_create();
    if (string_array_size(instruction_array) == 3) {
        t_instruction *instruction = safe_malloc(sizeof (t_instruction));
        instruction -> type = NO_OP;
        t_list *operands = list_create();
        for (int i = 1; instruction_array[i] ; ++i) {
            list_add(operands, atoi(instruction_array[i]));
        }
        instruction -> operands = operands;
        list_add(instructions, instruction);
        return instructions;
    } else {
        log_invalid_number_of_params(instruction_array[0], 2, string_array_size(instruction_array) - 1);
        free_system();
    }
}

t_list *create_exit_instruction (char** instruction_array) {
    t_list *instructions = list_create();
    if (string_array_size(instruction_array) == 1) {
        t_instruction *instruction = safe_malloc(sizeof (t_instruction));
        instruction -> type = EXIT;
        t_list *operands = list_create();
        for (int i = 1; instruction_array[i] ; ++i) {
            list_add(operands, atoi(instruction_array[i]));
        }
        instruction -> operands = operands;
        list_add(instructions, instruction);
        return instructions;
    } else {
        log_invalid_number_of_params(instruction_array[0], 0, string_array_size(instruction_array));
        free_system();
    }
}

void initialize_no_op_creator(t_list *(*function) (char**)) {
    t_instruction_creator *no_op_creator = safe_malloc(sizeof (t_instruction_creator));
    no_op_creator -> code_as_string = "NO_OP";
    no_op_creator -> function = function;
    list_add(instruction_creators, no_op_creator);
}

void initialize_io_creator(t_list *(*function) (char**)) {
    t_instruction_creator *io_creator = safe_malloc(sizeof (t_instruction_creator));
    io_creator -> code_as_string = "I/O";
    io_creator -> function = function;
    list_add(instruction_creators, io_creator);
}

void initialize_write_creator(t_list *(*function) (char**)) {
    t_instruction_creator *write_creator = safe_malloc(sizeof (t_instruction_creator));
    write_creator -> code_as_string = "WRITE";
    write_creator -> function = function;
    list_add(instruction_creators, write_creator);
}

void initialize_read_creator(t_list *(*function) (char**)) {
    t_instruction_creator *read_creator = safe_malloc(sizeof (t_instruction_creator));
    read_creator -> code_as_string = "READ";
    read_creator -> function = function;
    list_add(instruction_creators, read_creator);
}

void initialize_copy_creator(t_list *(*function) (char**)) {
    t_instruction_creator *copy_creator = safe_malloc(sizeof (t_instruction_creator));
    copy_creator -> code_as_string = "COPY";
    copy_creator -> function = function;
    list_add(instruction_creators, copy_creator);
}

void initialize_exit_creator(t_list *(*function) (char**)) {
    t_instruction_creator *exit_creator = safe_malloc(sizeof (t_instruction_creator));
    exit_creator -> code_as_string = "EXIT";
    exit_creator -> function = function;
    list_add(instruction_creators, exit_creator);
}

void initialize_instruction_creator() {
    instruction_creators = list_create();
    initialize_no_op_creator(create_no_op_instruction);
    initialize_io_creator(create_io_instruction);
    initialize_write_creator(create_write_instruction);
    initialize_read_creator(create_read_instruction);
    initialize_copy_creator(create_copy_instruction);
    initialize_exit_creator(create_exit_instruction);
}

t_list *create_instruction_from(char** instruction_array) {

    char* code = instruction_array[0];

    bool _is_for(t_instruction_creator * creator){
        return
                string_equals_ignore_case(code, creator -> code_as_string);
    }

    t_instruction_creator *creator_found = list_find(instruction_creators, _is_for);

    if(!creator_found){
        log_invalid_transition_error();
        free_system();
    }

    return creator_found -> function (instruction_array);
}

void free_instruction_creator() {
    list_destroy_and_destroy_elements(instruction_creators, free);
}