#ifndef KISS_CPU_QUERY_PERFORMER_H
#define KISS_CPU_QUERY_PERFORMER_H

#include <stdint.h>
#include "cpu_manager.h"
#include "../../Utils/include/serializable_objects.h"

typedef struct query_performer{
    uint32_t operation;
    void* (*perform_function) (t_request * request);
}t_query_performer;

void initialize_cpu_query_performers();
t_query_performer *query_performer_with_code(uint32_t operation_code);
void free_cpu_query_performers();

#endif //KISS_CPU_QUERY_PERFORMER_H
