//#include "../include/operation_deserialization.h"
#include <common_structures.h>
#include <string.h>
#include <serialization_interface.h>
#include <serializable_objects.h>
#include <stdlib.h>
#include <garbage_collector.h>

t_deserialization_information* deserialization_information_of(void* serialized_request){

    uint32_t operation;
    void* serialized_structure;
    uint32_t serialized_structure_size;

    int offset = 0;

    memcpy(&operation, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&serialized_structure_size, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    serialized_structure = safe_malloc(serialized_structure_size);
    memcpy(serialized_structure, serialized_request + offset, serialized_structure_size);

    t_deserialization_information* deserialization_information = safe_malloc(sizeof(t_deserialization_information));
    deserialization_information -> operation = operation;
    deserialization_information -> serialized_structure = serialized_structure;
    deserialization_information -> serialized_structure_size = serialized_structure_size;

    return deserialization_information;
}

t_request* deserialize_with_deserialization_information(t_deserialization_information* deserialization_information){

    t_serializable_object* serializable_object = serializable_object_with_code(deserialization_information -> operation);
    return (serializable_object -> deserialize_function) (deserialization_information -> serialized_structure);
}

t_request* deserialize(void* serialized_request){

    t_deserialization_information* deserialization_information = deserialization_information_of(serialized_request);
    t_request* request = deserialize_with_deserialization_information(deserialization_information);

    free_deserialization_information(deserialization_information);
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