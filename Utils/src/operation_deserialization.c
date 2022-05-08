#include "../include/operation_deserialization.h"
#include <common_structures.h>
#include <string.h>
#include <serialization_interface.h>
#include <serializable_objects.h>
#include <stdlib.h>
#include <garbage_collector.h>

t_deserialization_information *deserialization_information_of(void *serialized_request) {

    uint32_t operation;
    void *serialized_structure;
    uint32_t serialized_structure_size;

    int offset = 0;

    memcpy(&operation, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&serialized_structure_size, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    serialized_structure = safe_malloc(serialized_structure_size);
    memcpy(serialized_structure, serialized_request + offset, serialized_structure_size);

    t_deserialization_information *deserialization_information = safe_malloc(sizeof(t_deserialization_information));
    deserialization_information->operation = operation;
    deserialization_information->serialized_structure = serialized_structure;
    deserialization_information->serialized_structure_size = serialized_structure_size;

    return deserialization_information;
}

t_request *deserialize_with_deserialization_information(t_deserialization_information *deserialization_information) {

    t_serializable_object *serializable_object = serializable_object_with_code(deserialization_information->operation);
    return (serializable_object->deserialize_function)(deserialization_information->serialized_structure);
}

t_request *deserialize(void *serialized_request) {

    t_deserialization_information *deserialization_information = deserialization_information_of(serialized_request);
    t_request *request = deserialize_with_deserialization_information(deserialization_information);

    free_deserialization_information(deserialization_information);
    return request;
}

t_request *deserialize_console_message(void *serialized_structure) {

    uint32_t process_size;
    uint32_t instructions_size;
    t_list *instructions = list_create();
    uint32_t operands_size;
    uint32_t operand_value;
    uint32_t offset = 0;

    memcpy(&process_size, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&instructions_size, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    for (int i = 0; i < instructions_size; ++i) {
        t_instruction *instruction = safe_malloc(sizeof(t_instruction));
        memcpy(&instruction->type, serialized_structure + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(&operands_size, serialized_structure + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        t_list *operands = list_create();
        for (int j = 0; j < operands_size; ++j) {
            memcpy(&operand_value, serialized_structure + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
            list_add(operands, operand_value);
        }
        instruction->operands = operands;
        list_add(instructions, instruction);
    }

    t_console_message *console_message = safe_malloc(sizeof(t_console_message));
    console_message->process_size = process_size;
    console_message->instructions = instructions;

    t_request *request = safe_malloc(sizeof(t_request));
    request->operation = CONSOLE_MESSAGE;
    request->structure = (void *) console_message;
    request->sanitizer_function = free;

    consider_as_garbage(console_message, free);
    return request;
}

t_request *deserialize_instruction(void *serialized_structure) {

    uint32_t type;
    uint32_t operands_size;
    t_list *operands = list_create();
    uint32_t offset = 0;
    uint32_t operand_value;

    memcpy(&type, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&operands_size, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for (int i = 0; i < operands_size; ++i) {
        memcpy(&operand_value, serialized_structure + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        list_add(operands, operand_value);
    }

    t_instruction *instruction = safe_malloc(sizeof(t_instruction));
    instruction->type = type;
    instruction->operands = operands;

    t_request *request = safe_malloc(sizeof(t_request));
    request->operation = INSTRUCTION;
    request->structure = (void *) instruction;
    request->sanitizer_function = free;

    consider_as_garbage(instruction, free);
    return request;
}