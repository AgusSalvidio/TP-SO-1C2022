#ifndef KISS_CPU_ACTION_PERFORMER_H
#define KISS_CPU_ACTION_PERFORMER_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct Action_Performer{
    uint32_t operation;
    void* (*perform_function) (uint32_t pid, t_list* operands);
}t_action_performer;

void initialize_action_performers();
void free_action_performers();
t_action_performer* action_performer_with_code(uint32_t operation_code);

#endif //KISS_CPU_ACTION_PERFORMER_H
