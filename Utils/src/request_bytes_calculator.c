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
    return sizeof(uint32_t) +       //frame
           sizeof(uint32_t);       //offset

}

uint32_t amount_of_bytes_of_write(){
    return sizeof(uint32_t) +        //frame
           sizeof(uint32_t) +       //offset
           sizeof(uint32_t);       //value
}

uint32_t amount_of_bytes_of_copy(){
    return sizeof(uint32_t) +        //frame
           sizeof(uint32_t) +       //offset
           sizeof(uint32_t);       //value
}

uint32_t amount_of_bytes_of_pcb(void *structure) {
    t_pcb *pcb = (t_pcb *) structure;
    uint32_t amount_of_bytes = sizeof(uint32_t) + //pid
                               sizeof(uint32_t) + //process_size
                               sizeof(uint32_t); //instruction length

    for (int i = 0; i < list_size(pcb->instructions); ++i) {
        amount_of_bytes = amount_of_bytes + amount_of_bytes_of_instruction(list_get(pcb->instructions, i));
    }

    amount_of_bytes = amount_of_bytes +
                      sizeof(uint32_t) + // pc
                      sizeof(uint32_t) + // page_table
                      sizeof(double); // next_burst

    return amount_of_bytes;
}

uint32_t amount_of_bytes_of_io_pcb(void *structure) {
    t_io_pcb* io_pcb = (t_io_pcb*)structure;
    t_pcb *pcb = io_pcb -> pcb;
    uint32_t amount_of_bytes = sizeof(uint32_t) + //pid
                               sizeof(uint32_t) + //process_size
                               sizeof(uint32_t); //instruction length

    for (int i = 0; i < list_size(pcb->instructions); ++i) {
        amount_of_bytes = amount_of_bytes + amount_of_bytes_of_instruction(list_get(pcb->instructions, i));
    }

    amount_of_bytes = amount_of_bytes +
                      sizeof(uint32_t) + // pc
                      sizeof(uint32_t) + // page_table
                      sizeof(double)+ // next_burst
                      sizeof(uint32_t); //blocked_time;

    return amount_of_bytes;
}

uint32_t amount_of_bytes_of_mmu_access() {
    return sizeof(uint32_t) +        //type
           sizeof(uint32_t) +       //index
           sizeof(uint32_t);       //entry
}