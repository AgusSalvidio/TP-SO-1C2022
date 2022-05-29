#ifndef KISS_CONSOLE_INSTRUCTION_HANDLER_H
#define KISS_CONSOLE_INSTRUCTION_HANDLER_H

#include "stdint.h"
#include "../../Utils/include/common_structures.h"

typedef struct Instruction_Creator{
    char* code_as_string;
    uint32_t code;
    t_instruction* (*function) (char** instruction_array);
}t_instruction_creator;

void initialize_instruction_creator();

t_instruction* create_instruction_from(char** instruction_array);

void free_instruction_creator();

#endif //KISS_CONSOLE_INSTRUCTION_HANDLER_H
