#include <string.h>
#include <common_structures.h>
#include "request_bytes_calculator.h"

uint32_t amount_of_bytes_of_console_message(void *structure) {
    t_console_message *console_message = (t_console_message *) structure;
    return sizeof(uint32_t) // process size
    + sizeof(uint32_t) // instructions length
    + strlen(console_message->instructions);
}

uint32_t amount_of_bytes_of_instruction(void* structure) {
    t_instruction *instruction = (t_instruction*) structure;
    return sizeof(uint32_t) // type
           + sizeof (uint32_t) // operands size
           + sizeof (uint32_t) * list_size(instruction -> operands);
}