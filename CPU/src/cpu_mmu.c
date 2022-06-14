#include <cpu_mmu.h>
#include "cpu_memory_connection_handler.h"
#include <stdint.h>
#include <math.h>

t_level_paging* level_paging_information(uint32_t logical_address){
    uint32_t page_number, first_level_entry, second_level_entry, offset;
    t_handshake* handshake_information = get_handshake_information();

    page_number = floor(logical_address / handshake_information->page_size);
    first_level_entry = floor(page_number / handshake_information->entries_per_page);
    second_level_entry = page_number % (handshake_information->entries_per_page);
    offset = logical_address - page_number * handshake_information->page_size;

    t_level_paging* two_level_paging = safe_malloc(sizeof(t_level_paging));
    two_level_paging -> first_level_entry = first_level_entry;
    two_level_paging -> second_level_entry = second_level_entry;
    two_level_paging -> offset = offset;

    return two_level_paging;
}

t_physical_address * physical_address_translator(uint32_t first_level_index, uint32_t logical_address){

    t_level_paging* level_paging = level_paging_information(logical_address);

    send_mmu_access_to_memory(FIRST_ACCES, first_level_index, level_paging->first_level_entry);
    uint32_t second_level_index = (uint32_t) receive_content_from_memory();

    send_mmu_access_to_memory(SECOND_ACCESS, second_level_index, level_paging->second_level_entry);
    uint32_t frame = (uint32_t) receive_content_from_memory();

    t_physical_address* physical_address = safe_malloc(sizeof(t_physical_address));
    physical_address -> frame = frame;
    physical_address -> offset = level_paging -> offset;

    return physical_address;
}
