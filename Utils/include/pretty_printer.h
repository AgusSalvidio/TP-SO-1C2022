#ifndef KISS_PRETTY_PRINTER_H
#define KISS_PRETTY_PRINTER_H

#include <stdint.h>
#include "common_structures.h"

typedef struct Printable_Object{
    uint32_t code;
    char* code_as_string;
    char* (*print_function) (void* object);
}t_printable_object;

void initialize_pretty_printer();

char* pretty_print_of(uint32_t code, void* structure);

char* operation_code_as_string(uint32_t code);

char* request_pretty_print(t_request* request);

char* reset_colour();

char* blue_colour();

char* change_message_colour(char* message, char* colour);

void free_pretty_printer();

#endif //KISS_PRETTY_PRINTER_H
