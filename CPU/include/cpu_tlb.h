#ifndef KISS_CPU_TLB_H
#define KISS_CPU_TLB_H

#include <stdint.h>
#include "cpu_manager.h"

uint32_t tlb_hit(uint32_t page_number);
bool page_number_contained_in_tlb(uint32_t page_number);
void add_new_element_to_tlb(uint32_t page_number, uint32_t frame_number);
void initialize_tlb();

#endif //KISS_CPU_TLB_H
