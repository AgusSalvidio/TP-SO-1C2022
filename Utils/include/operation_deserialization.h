#ifndef KISS_OPERATION_DESERIALIZATION_H
#define KISS_OPERATION_DESERIALIZATION_H

#include "serialization_interface.h"

t_request* deserialize_console_message(void* serialized_structure);
t_request* deserialize_instruction(void* serialized_structure);

t_request* deserialize_initialize_process(void* serialized_structure);
t_request* deserialize_suspend_process(void* serialized_structure);
t_request* deserialize_finalize_process(void* serialized_structure);

t_request* deserialize_pcb(void* serialized_structure);

#endif //KISS_OPERATION_DESERIALIZATION_H
