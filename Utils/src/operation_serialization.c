#include <common_structures.h>
#include <string.h>
#include <request_bytes_calculator.h>
#include "../include/operation_serialization.h"
#include "../include/serializable_objects.h"

t_serialization_information* serialize(t_request* request){

    t_serializable_object* serializable_object = serializable_object_with_code(request -> operation);
    return (serializable_object -> serialize_function) (request -> structure);
}

void serialize_instruction_structure(void* serialized_request, uint32_t* offset, t_instruction* instruction) {
    uint32_t operands_size = list_size(instruction->operands);
    memcpy(serialized_request + *offset, &(instruction->type), sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    memcpy(serialized_request + *offset, &operands_size, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    for (int i = 0; i < operands_size; ++i) {
        uint32_t operand = (uint32_t) list_get(instruction->operands, i);
        memcpy(serialized_request + *offset, &operand, sizeof(uint32_t));
        *offset += sizeof(uint32_t);
    }

}

t_serialization_information* serialize_console_message(void* structure) {
    t_console_message* console_message = (t_console_message*) structure;
    uint32_t amount_of_bytes = amount_of_bytes_of_console_message(structure);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = CONSOLE_MESSAGE;
    uint32_t instructions_size = list_size(console_message->instructions);
    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(console_message -> process_size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &instructions_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    for (int i = 0; i < instructions_size; ++i) {
        t_instruction *instruction = list_get(console_message->instructions, i);
        serialize_instruction_structure(serialized_request, &offset, instruction);
    }

    memcpy(serialized_request + offset, console_message -> instructions, instructions_size);

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}




t_serialization_information* serialize_instruction(void* structure) {
    t_instruction* instruction = (t_instruction*) structure;
    uint32_t amount_of_bytes = amount_of_bytes_of_instruction(structure);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // code
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = INSTRUCTION;
    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    serialize_instruction_structure(serialized_request, &offset, instruction);


    t_serialization_information
    * serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}