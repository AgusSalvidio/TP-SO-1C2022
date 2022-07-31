#ifndef KISS_SERIALIZABLE_OBJECTS_H
#define KISS_SERIALIZABLE_OBJECTS_H

#include "serialization_interface.h"

void initialize_serializable_objects();

t_serializable_object* serializable_object_with_code(uint32_t operation_code);


t_serializable_object* serializable_handshake();
t_serializable_object* serializable_request_response();
t_serializable_object* serializable_read();
t_serializable_object* serializable_write();
t_serializable_object* serializable_copy();
t_serializable_object* serializable_first_access();
t_serializable_object* serializable_second_access();

void free_serializable_objects();

#endif //KISS_SERIALIZABLE_OBJECTS_H
