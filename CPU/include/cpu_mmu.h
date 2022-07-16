#ifndef CPU_MMU_H
#define CPU_MMU_H

#include "../../Utils/include/common_structures.h"

t_physical_address* address_translator_management(uint32_t first_level_index, uint32_t logical_address);

#endif //COMMON_STRUCTURES_H