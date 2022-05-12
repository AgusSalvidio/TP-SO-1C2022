#include <string.h>
#include <common_structures.h>
#include "request_bytes_calculator.h"



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