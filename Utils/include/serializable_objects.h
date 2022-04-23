#ifndef KISS_SERIALIZABLE_OBJECTS_H
#define KISS_SERIALIZABLE_OBJECTS_H

#include "serialization_interface.h"

void initialize_serializable_objects();

t_serializable_object* serializable_object_with_code(uint32_t operation_code);

void free_serializable_objects();

#endif //KISS_SERIALIZABLE_OBJECTS_H
