#include <cpu_mmu.h>
#include "cpu_memory_connection_handler.h"
#include "cpu_manager.h"
#include "cpu_tlb.h"
#include <stdint.h>
#include <math.h>

uint32_t page_number, offset_number;

t_level_paging* level_paging_information(uint32_t logical_address){
    uint32_t first_level_entry, second_level_entry;
    t_handshake* handshake_information = get_handshake_information();

    page_number = floor(logical_address / handshake_information->page_size);
    first_level_entry = floor(page_number / handshake_information->entries_per_page);
    second_level_entry = page_number % (handshake_information->entries_per_page);
    offset_number = logical_address - page_number * handshake_information->page_size;

    t_level_paging* two_level_paging = safe_malloc(sizeof(t_level_paging));
    two_level_paging -> first_level_entry = first_level_entry;
    two_level_paging -> second_level_entry = second_level_entry;
    two_level_paging -> offset = offset_number;

    return two_level_paging;
}

t_physical_address * physical_address_translator(uint32_t first_level_index, uint32_t logical_address){

    t_level_paging* level_paging = level_paging_information(logical_address);

    send_mmu_access_to_memory(FIRST_ACCESS, first_level_index, level_paging->first_level_entry);
    uint32_t second_level_index = receive_content_from_memory();

    send_mmu_access_to_memory(SECOND_ACCESS, second_level_index, level_paging->second_level_entry);
    uint32_t frame = receive_content_from_memory();

    t_physical_address* physical_address = safe_malloc(sizeof(t_physical_address));
    physical_address -> frame = frame;
    physical_address -> offset = level_paging -> offset;

    add_new_element_to_tlb(page_number, frame);

    return physical_address;
}

t_physical_address* physical_address_tlb_translator(uint32_t frame_number, uint32_t offset){

    t_physical_address* physical_address = safe_malloc(sizeof(t_physical_address));
    physical_address -> frame = frame_number;
    physical_address -> offset = offset;

    return physical_address;
}

t_physical_address* address_translator_management(uint32_t first_level_index, uint32_t logical_address){
    uint32_t frame_number;
    t_handshake* handshake_information = get_handshake_information();

    page_number = floor(logical_address / handshake_information->page_size);
    offset_number = logical_address - page_number * handshake_information->page_size;

    if(page_number_contained_in_tlb(page_number)){
        frame_number = tlb_hit(page_number);
        return physical_address_tlb_translator(frame_number, offset_number);
    }

    return physical_address_translator(first_level_index, logical_address);
}
