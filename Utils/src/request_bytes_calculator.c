#include <string.h>
#include <common_structures.h>
#include "request_bytes_calculator.h"


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