#include <pretty_printer.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <serializable_objects.h>
#include <queue_code_name_associations.h>
#include <general_logs.h>
#include <garbage_collector.h>

t_list* printable_objects;

char* read_page_as_string(t_read * read) {
    return string_from_format("Operación: READ\nSe lee %d",
                              read -> logical_address);
}

void initialize_and_load_read() {
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = READ;
    printable_object -> code_as_string = "READ";
    printable_object -> print_function = (char *(*)(void *)) read_page_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_pretty_printer(){

    printable_objects = list_create();

    initialize_and_load_read();

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