#ifndef KISS_OPERATION_SERIALIZATION_H
#define KISS_OPERATION_SERIALIZATION_H

t_serialization_information* serialize_init(void* structure);
t_serialization_information* serialize_close(void* structure);
t_serialization_information* serialize_sem_init(void* structure);
t_serialization_information* serialize_sem_wait(void* structure);
t_serialization_information* serialize_sem_post(void* structure);
t_serialization_information* serialize_sem_destroy(void* structure);
t_serialization_information* serialize_call_io(void* structure);

t_serialization_information * serialize_read(void *structure);
t_serialization_information* serialize_write(void* structure);
t_serialization_information* serialize_copy(void* structure);

#endif //KISS_OPERATION_SERIALIZATION_H