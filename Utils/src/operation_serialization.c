#include <common_structures.h>
#include <string.h>
#include <request_bytes_calculator.h>
#include "../include/operation_serialization.h"
#include "../include/serializable_objects.h"

t_serialization_information* serialize(t_request* request){

    t_serializable_object* serializable_object = serializable_object_with_code(request -> operation);
    return (serializable_object -> serialize_function) (request -> structure);
}

t_serialization_information* serialize_handshake(void* structure){

    t_handshake * handshake = (t_handshake*) structure;
    uint32_t bytes_of_handshake = amount_of_bytes_of_handshake();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + bytes_of_handshake;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = HANDSHAKE;
    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &bytes_of_handshake, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(handshake -> page_size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(handshake -> entries_per_page), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}
   
t_serialization_information * serialize_read(void *structure) {

    uint32_t operation = READ;
    t_read * read = (t_read*) structure;
    uint32_t amount_of_bytes_of_struct = amount_of_bytes_of_read();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_struct;          // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_struct, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(read -> logical_address), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_request_response(void* structure){

    t_request_response * request_response = (t_request_response *) structure;
    uint32_t bytes_of_request_response = amount_of_bytes_of_request_response(structure);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + bytes_of_request_response;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = REQUEST_RESPONSE;
    uint32_t offset = 0;
    uint32_t type_description_lenght = strlen(request_response -> type_description);
    uint32_t content_lenght = strlen(request_response -> content);


    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &bytes_of_request_response, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &type_description_lenght, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(request_response -> type_description), type_description_lenght);
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &content_lenght, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(request_response -> content), content_lenght);
    
    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}
  
t_serialization_information* serialize_write(void* structure){

    t_write * write = (t_write*) structure;
    uint32_t amount_of_bytes_of_struct = amount_of_bytes_of_write();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_struct;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = WRITE;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_struct, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(write -> logical_address), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(write -> value), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}
t_serialization_information* serialize_copy(void* structure){

    t_copy * copy = (t_copy*) structure;
    uint32_t amount_of_bytes_of_struct = amount_of_bytes_of_copy();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_struct;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = COPY;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_struct, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(copy -> destiny_logical_address), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(copy -> origin_logical_address), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;

}
