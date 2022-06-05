#ifndef KISS_KERNEL_STATE_TRANSITIONS_H
#define KISS_KERNEL_STATE_TRANSITIONS_H

#include <stdint.h>
#include "kernel_process_image.h"
#include "../../Utils/include/common_structures.h"

typedef struct State_transition{
    uint32_t from_state;
    uint32_t to_state;
    void (*function) (t_pcb* pcb);
}t_state_transition;

void initialize_state_transitions();

t_state_transition* state_transition_for(t_pcb *pcb, uint32_t to_state);

void free_state_transitions();

#endif //KISS_KERNEL_STATE_TRANSITIONS_H
