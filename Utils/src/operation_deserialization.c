//#include "../include/operation_deserialization.h"
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

t_request* deserialize_handshake(void* serialized_structure) {

    uint32_t page_size;
    uint32_t entries_per_page;

    uint32_t offset = 0;

    memcpy(&page_size, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&entries_per_page, serialized_structure + offset, sizeof(uint32_t));

    t_handshake * handshake = safe_malloc(sizeof(t_handshake));
    handshake -> page_size = page_size;
    handshake -> entries_per_page = entries_per_page;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = HANDSHAKE;
    request -> structure = (void*) handshake;
    request -> sanitizer_function = free;

    consider_as_garbage(handshake, free);
    return request;
}

t_request* deserialize_request_response(void* serialized_structure){

    char* type_description;
    uint32_t type_description_lenght;
    uint32_t content;
    uint32_t offset = 0;

    memcpy(&type_description_lenght, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    uint32_t type_description_length_with_trailing_null = type_description_lenght + 2;
    type_description = calloc(type_description_length_with_trailing_null, sizeof(char));
    memcpy(type_description, serialized_structure + offset, type_description_lenght);
    offset += strlen(type_description);

    memcpy(&content, serialized_structure + offset, sizeof(uint32_t));


    t_request_response * request_response = safe_malloc(sizeof(t_request_response));
    request_response -> type_description = type_description;
    request_response -> content = content;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = REQUEST_RESPONSE;
    request -> structure = (void*) request_response;
    request -> sanitizer_function = free;

    consider_as_garbage(request_response, free);
    return request;
}
t_request *deserialize_read(void *serialized_structure) {
    uint32_t operation = READ;
    uint32_t logical_adress;

    memcpy(&logical_adress, serialized_structure, sizeof(uint32_t));

    t_read* read_to_deserialize = safe_malloc(sizeof(t_read));
    read_to_deserialize -> logical_address = logical_adress;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = operation;
    request -> structure = (void*) read_to_deserialize;
    request -> sanitizer_function = free;

    return request;
}
t_request* deserialize_write(void* serialized_structure) {

    uint32_t logical_adress;
    uint32_t value;

    uint32_t offset = 0;

    memcpy(&logical_adress, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&value, serialized_structure + offset, sizeof(uint32_t));

    t_write* write = safe_malloc(sizeof(t_write));
    write -> logical_address = logical_adress;
    write -> value = value;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = WRITE;
    request -> structure = (void*) write;
    request -> sanitizer_function = free;

    consider_as_garbage(write, free);
    return request;
}
t_request* deserialize_copy(void* serialized_structure) {

    uint32_t destiny_logical_adress;
    uint32_t origin_logical_adress;

    uint32_t offset = 0;

    memcpy(&destiny_logical_adress, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&origin_logical_adress, serialized_structure + offset, sizeof(uint32_t));

    t_copy* copy = safe_malloc(sizeof(t_copy));
    copy -> destiny_logical_address = destiny_logical_adress;
    copy -> origin_logical_address = origin_logical_adress;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = COPY;
    request -> structure = (void*) copy;
    request -> sanitizer_function = free;

    consider_as_garbage(copy, free);
    return request;
}