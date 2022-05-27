#include <pretty_printer.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <serializable_objects.h>
#include <queue_code_name_associations.h>
#include <general_logs.h>
#include <garbage_collector.h>

t_list* printable_objects;

char *instruction_structure_as_string(t_instruction *instruction) {
    char* operands_as_string = string_new();
    for (int i = 0; i < list_size(instruction->operands); ++i) {
        uint32_t operand = (uint32_t) list_get(instruction->operands, i);
        char *operand_str = string_itoa(operand);
        string_append(&operands_as_string, operand_str);
        free(operand_str);
        string_append(&operands_as_string, ", ");
    }
    char *string = string_from_format("Type: %d, Operands: [%s]", instruction->type, operands_as_string);
    free(operands_as_string);
    return string;
}

char* instruction_as_string(t_instruction * instruction){

    char *operands_as_string = instruction_structure_as_string(instruction);

    return string_from_format("Operación: INSTRUCTION\nArgumentos: %s",
                              operands_as_string);
}

void initialize_and_load_instruction_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = INSTRUCTION;
    printable_object -> code_as_string = "INSTRUCTION";
    printable_object -> print_function = (char *(*)(void *)) instruction_as_string;

    list_add(printable_objects, (void*) printable_object);
}

char* console_message_as_string(t_console_message * console_message){

    char* instructions_as_string = string_new();
    for (int i = 0; i < list_size(console_message->instructions); ++i) {
        t_instruction * instruction = list_get(console_message->instructions, i);
        char *struct_as_string = instruction_structure_as_string(instruction);
        string_append(&instructions_as_string, struct_as_string);
        free(struct_as_string);
        string_append(&instructions_as_string, ", ");
    }

    char *string = string_from_format("Operación: CONSOLE_MESSAGE\nArgumentos: Process_size: %d, Instructions: [%s]",
                                      console_message->process_size, instructions_as_string);
    free(instructions_as_string);
    return string;
}

void initialize_and_load_console_message_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = CONSOLE_MESSAGE;
    printable_object -> code_as_string = "CONSOLE_MESSAGE";
    printable_object -> print_function = (char *(*)(void *)) console_message_as_string;
    list_add(printable_objects, (void*) printable_object);
}

char* initialize_process_as_string(t_initialize_process *initialize_process){

    return string_from_format("Operación: INITIALIZE_PROCESS\nPid: %d, Process_size: %d.",
                              initialize_process->pid, initialize_process->process_size);
}

void initialize_and_load_initialize_process_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = INITIALIZE_PROCESS;
    printable_object -> code_as_string = "INITIALIZE_PROCESS";
    printable_object -> print_function = (char *(*)(void *)) initialize_process_as_string;
    list_add(printable_objects, (void*) printable_object);
}

char* suspend_process_as_string(t_suspend_process *suspend_process){

    return string_from_format("Operación: SUSPEND_PROCESS\nPid: %d.",
                              suspend_process->pid);
}

void initialize_and_load_suspend_process_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = SUSPEND_PROCESS;
    printable_object -> code_as_string = "SUSPEND_PROCESS";
    printable_object -> print_function = (char *(*)(void *)) suspend_process_as_string;
    list_add(printable_objects, (void*) printable_object);
}

void initialize_pretty_printer(){

    printable_objects = list_create();

    initialize_and_load_console_message_pretty_print();
    initialize_and_load_instruction_pretty_print();
    initialize_and_load_initialize_process_pretty_print();
    initialize_and_load_suspend_process_pretty_print();


    log_succesful_initialize_pretty_printer();
}

t_printable_object* printable_object_with_code(uint32_t code){

    bool _has_code(void* printable_object){
        t_printable_object* cast_printable_object = (t_printable_object*) printable_object;
        return cast_printable_object -> code == code;
    }

    t_printable_object* printable_object_found = list_find(printable_objects, _has_code);

    if(printable_object_found == NULL){
        log_printable_object_not_found_error();
        free_system();
    }

    return printable_object_found;
}

char* pretty_print_of(uint32_t code, void* structure){

    t_printable_object* printable_object = printable_object_with_code(code);
    return printable_object -> print_function (structure);
}

char* operation_code_as_string(uint32_t code){

    t_printable_object* printable_object = printable_object_with_code(code);
    return printable_object -> code_as_string;
}

char* request_pretty_print(t_request* request){
    return pretty_print_of(request -> operation, request -> structure);
}

char* reset_colour(){
    return "\x1b[0m";
}

char* blue_colour(){
    return "\x1b[36m";
}

char* change_message_colour(char* message, char* colour){
    return string_from_format("%s%s%s", colour, message, reset_colour());
}

void free_pretty_printer(){
    list_destroy_and_destroy_elements(printable_objects, free);
}