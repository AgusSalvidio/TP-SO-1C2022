#include <common_structures.h>
#include "request_bytes_calculator.h"

uint32_t amount_of_bytes_of_console_message(void *structure) {
    t_console_message *console_message = (t_console_message *) structure;
    uint32_t amount_of_bytes = sizeof(uint32_t) // process size
                               + sizeof(uint32_t); // instructions length

    for (int i = 0; i < list_size(console_message->instructions); ++i) {
        amount_of_bytes = amount_of_bytes + amount_of_bytes_of_instruction(list_get(console_message->instructions, i));
    }

    return amount_of_bytes;
}

uint32_t amount_of_bytes_of_instruction(void *structure) {
    t_instruction *instruction = (t_instruction *) structure;
    return sizeof(uint32_t) // type
           + sizeof(uint32_t) // operands size
           + sizeof(uint32_t) * list_size(instruction->operands);
}

uint32_t amount_of_bytes_of_handshake(){
    return sizeof(uint32_t) +       //page_size
           sizeof(uint32_t);               //entries_per_page
}

uint32_t amount_of_bytes_of_request_response(void* structure){
    t_request_response *request_response = (t_request_response *) structure;
    return sizeof(uint32_t) +                                   //type_description lenght
           strlen(request_response -> type_description) +    //type_description
           sizeof(uint32_t) +                                  //content lenght
           strlen(request_response -> content);             //content
}
  
uint32_t amount_of_bytes_of_read(){
    return sizeof(uint32_t);       //logical_adress
}

uint32_t amount_of_bytes_of_write(){
    return sizeof(uint32_t) +       //logical_adress
           sizeof(uint32_t);       //value
}

uint32_t amount_of_bytes_of_copy(){
    return sizeof(uint32_t) +       //destiny_logical_adress
           sizeof(uint32_t);       //origin_logical_adress
}