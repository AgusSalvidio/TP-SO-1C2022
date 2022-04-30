#include "../include/operation_deserialization.h"
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