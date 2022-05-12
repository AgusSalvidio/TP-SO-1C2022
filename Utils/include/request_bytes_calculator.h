#ifndef KISS_REQUEST_BYTES_CALCULATOR_H
#define KISS_REQUEST_BYTES_CALCULATOR_H


uint32_t amount_of_bytes_of_handshake();
uint32_t amount_of_bytes_of_request_response(void* structure);
uint32_t amount_of_bytes_of_read();
uint32_t amount_of_bytes_of_write();
uint32_t amount_of_bytes_of_copy();


#endif //KISS_REQUEST_BYTES_CALCULATOR_H
