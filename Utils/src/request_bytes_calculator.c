#include <string.h>
#include <common_structures.h>
#include "request_bytes_calculator.h"

uint32_t amount_of_bytes_of_console_message(void *structure) {
    t_console_message *console_message = (t_console_message *) structure;
    return sizeof(uint32_t)
    + sizeof(uint32_t)
    + strlen(console_message->instructions);
}