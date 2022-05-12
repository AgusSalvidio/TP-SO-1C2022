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

t_request* deserialize_request_response(void* serialized_structure){

    char* type_description;
    uint32_t type_description_lenght;

    char* content;
    uint32_t content_lenght;


    uint32_t offset = 0;

    memcpy(&type_description_lenght, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    uint32_t type_description_length_with_trailing_null = type_description_lenght + 2;
    type_description = calloc(type_description_length_with_trailing_null, sizeof(char));
    memcpy(type_description, serialized_structure + offset, type_description_lenght);
    offset += strlen(type_description);

    memcpy(&content_lenght, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    uint32_t content_length_with_trailing_null = content_lenght + 2;
    content = calloc(content_length_with_trailing_null, sizeof(char));
    memcpy(content, serialized_structure + offset, content_lenght);


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