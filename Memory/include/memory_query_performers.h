#ifndef KISS_MEMORY_QUERY_PERFORMERS_H
#define KISS_MEMORY_QUERY_PERFORMERS_H

#include "memory_configuration_manager.h"

typedef struct query_performer{
    uint32_t operation;
    void* (*perform_function) (t_request * request);
}t_query_performer;

void initialize_memory_query_performers();
void free_memory_query_performers();
t_query_performer* query_performer_handle(uint32_t operation);
t_query_performer* query_performer_with_code(uint32_t operation_code);


#endif //KISS_MEMORY_QUERY_PERFORMERS_H
