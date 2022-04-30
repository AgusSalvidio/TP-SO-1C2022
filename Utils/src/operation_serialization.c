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
