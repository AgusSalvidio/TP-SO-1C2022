#ifndef KISS_OPERATION_SERIALIZATION_H
#define KISS_OPERATION_SERIALIZATION_H

t_serialization_information* serialize_console_message(void* structure);
t_serialization_information* serialize_instruction(void* structure);

t_serialization_information* serialize_initialize_process(void* structure);
t_serialization_information* serialize_suspend_process(void* structure);


t_serialization_information* serialize_handshake(void* structure);
t_serialization_information* serialize_request_response(void* structure);
t_serialization_information* serialize_read(void *structure);
t_serialization_information* serialize_write(void* structure);
t_serialization_information* serialize_copy(void* structure);


#endif //KISS_OPERATION_SERIALIZATION_H