#ifndef KISS_REQUEST_BYTES_CALCULATOR_H
#define KISS_REQUEST_BYTES_CALCULATOR_H

uint32_t amount_of_bytes_of_console_message(void* structure);
uint32_t amount_of_bytes_of_instruction(void* structure);
uint32_t amount_of_bytes_of_handshake();
uint32_t amount_of_bytes_of_request_response(void* structure);
uint32_t amount_of_bytes_of_read();
uint32_t amount_of_bytes_of_write();
uint32_t amount_of_bytes_of_copy();
uint32_t amount_of_bytes_of_initialize_process();
uint32_t amount_of_bytes_of_suspend_process();



#endif //KISS_REQUEST_BYTES_CALCULATOR_H
