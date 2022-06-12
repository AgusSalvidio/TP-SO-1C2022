#include <cpu_instruction_cycle.h>
#include "../../Utils/include/common_structures.h"
#include "cpu_action_performer.h"
#include "cpu_memory_connection_handler.h"

t_instruction* fetch_instruction(t_pcb* current_pcb){
    uint32_t program_counter = current_pcb -> pc;
    t_instruction* next_instruction = list_get(current_pcb -> instructions, program_counter);

    program_counter++;
    current_pcb -> pc = program_counter;

    return next_instruction;
}

uint32_t fetch_operands(uint32_t operand){
    send_read_to_memory(operand);
    uint32_t converted_content = (uint32_t) receive_read_content_from_memory();

    return converted_content;
}

t_instruction* decode_instruction(t_instruction* current_instruction){
    uint32_t value, operand;

    if(current_instruction -> type == COPY){
        operand = (uint32_t) list_get(current_instruction -> operands, 1);
        value = fetch_operands(operand);
        list_add_in_index(current_instruction -> operands, 1, (void*) value);
    }
    return current_instruction;
}

void execute(t_instruction* instruction){
    t_list* current_operands = instruction -> operands;
    t_action_performer* action_performer = action_performer_with_code(instruction -> type);

    (action_performer->perform_function) (current_operands);
}
void execute_instruction_cycle(t_pcb* current_pcb){

    t_instruction* instruction = fetch_instruction(current_pcb);
    decode_instruction(instruction);
    execute(instruction);
}
