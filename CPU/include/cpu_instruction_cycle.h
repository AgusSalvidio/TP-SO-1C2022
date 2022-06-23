#ifndef KISS_CPU_INSTRUCTION_CYCLE_H
#define KISS_CPU_INSTRUCTION_CYCLE_H

#include "../../Utils/include/common_structures.h"

t_instruction* fetch_instruction(t_pcb* current_pcb);
t_instruction* decode_instruction(uint32_t pid, t_instruction* current_instruction);
void execute(uint32_t table_index, t_instruction* instruction);

#endif //KISS_CPU_INSTRUCTION_CYCLE_H
