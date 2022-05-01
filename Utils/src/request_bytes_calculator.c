#include <string.h>
#include <common_structures.h>
#include "request_bytes_calculator.h"


uint32_t amount_of_bytes_of_handshake(){
    return sizeof(uint32_t) +       //page_size
           sizeof(uint32_t);               //entries_per_page
}