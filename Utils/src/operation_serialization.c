#include <common_structures.h>
#include <string.h>
#include <request_bytes_calculator.h>
#include "../include/operation_serialization.h"
#include "../include/serializable_objects.h"

t_serialization_information* serialize(t_request* request){

    t_serializable_object* serializable_object = serializable_object_with_code(request -> operation);
    return (serializable_object -> serialize_function) (request -> structure);
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
    memcpy(serialized_request + offset, &(read -> logical_adress), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_write(void* structure){

    t_write * write = (t_write*) structure;
    uint32_t amount_of_bytes_of_new_mem_alloc = amount_of_bytes_of_write();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_new_mem_alloc;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = WRITE;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_new_mem_alloc, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(write -> logical_adress), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(write -> value), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_copy(void* structure){

    t_copy * copy = (t_copy*) structure;
    uint32_t amount_of_bytes_of_new_mem_alloc = amount_of_bytes_of_copy();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_new_mem_alloc;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = COPY;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_new_mem_alloc, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(copy -> destiny_logical_adress), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(copy -> origin_logical_adress), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}