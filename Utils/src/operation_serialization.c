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

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
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
    t_physical_address* physical_address = read -> physical_address;
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
    memcpy(serialized_request + offset, &(physical_address -> frame), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(physical_address -> offset), sizeof(uint32_t));

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
    t_physical_address* physical_address = write->physical_address;
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
    memcpy(serialized_request + offset, &(physical_address -> frame), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(physical_address -> offset), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(write -> value), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}
  
t_serialization_information* serialize_copy(void* structure){

    t_copy * copy = (t_copy*) structure;
    t_physical_address* physical_address = copy->physical_address;
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
    memcpy(serialized_request + offset, &(physical_address -> frame), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(physical_address -> offset), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(copy -> value), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;

}

t_serialization_information* serialize_pcb(void* structure){

    t_pcb *pcb = (t_pcb*) structure;
    uint32_t amount_of_bytes_of_struct = amount_of_bytes_of_pcb(structure);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_struct;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = IO_PCB;
    uint32_t instructions_size = list_size(pcb->instructions);
    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_struct, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> pid), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> process_size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &instructions_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    for (int i = 0; i < instructions_size; ++i) {
        t_instruction *instruction = list_get(pcb->instructions, i);
        serialize_instruction_structure(serialized_request, &offset, instruction);
    }
    memcpy(serialized_request + offset, &(pcb -> pc), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> page_table), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> next_burst), sizeof(double));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;

}

t_serialization_information* serialize_io_pcb(void* structure){

    t_io_pcb* io_pcb = (t_io_pcb*)structure;
    t_pcb *pcb = io_pcb -> pcb;
    uint32_t amount_of_bytes_of_struct = amount_of_bytes_of_io_pcb(structure);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_struct;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = IO_PCB;
    uint32_t instructions_size = list_size(pcb->instructions);
    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_struct, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> pid), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> process_size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &instructions_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    for (int i = 0; i < instructions_size; ++i) {
        t_instruction *instruction = list_get(pcb->instructions, i);
        serialize_instruction_structure(serialized_request, &offset, instruction);
    }
    memcpy(serialized_request + offset, &(pcb -> pc), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> page_table), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(pcb -> next_burst), sizeof(double));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(io_pcb -> blocked_time), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;

}

t_serialization_information* serialize_mmu_access(void* structure){
    uint32_t operation = safe_malloc(sizeof(uint32_t));
    t_mmu_access * mmu_access = (t_mmu_access*) structure;
    uint32_t amount_of_bytes_of_struct = amount_of_bytes_of_mmu_access();
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
    memcpy(serialized_request + offset, &(mmu_access -> index), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(mmu_access -> entry), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}