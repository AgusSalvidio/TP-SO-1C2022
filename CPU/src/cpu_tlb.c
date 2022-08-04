#include <cpu_tlb.h>
#include <string.h>
#include <stdlib.h>
#include "cpu_manager.h"
#include "cpu_configuration_manager.h"
#include "cpu_logs_manager.h"

t_list* tlb_elements;
uint32_t tlb_order = 1;

uint32_t tlb_hit(uint32_t page_number){
    t_tlb_element* tlb_element = safe_malloc(sizeof(t_tlb_element));

    bool _list_contains_element(void* tlb_element) {
        return ((t_tlb_element *) tlb_element)->page_number == page_number;
    }

    tlb_element = list_find(tlb_elements, _list_contains_element);
    return tlb_element -> frame_number;
}

bool page_number_contained_in_tlb(uint32_t page_number){

    for(uint32_t counter = 0; counter < list_size(tlb_elements); counter++){
        t_tlb_element* tlb_element = list_get(tlb_elements, counter);

        if(tlb_element->page_number == page_number){
            return true;
        }
    }
    return false;
}

void fifo_tlb_replacement(t_tlb_element* element){

    bool _page_already_in_tlb(void* tlb_element){
        return ((t_tlb_element*) tlb_element) -> page_number == element -> page_number;
    }

    if(!list_any_satisfy(tlb_elements, _page_already_in_tlb)){
        if(list_size(tlb_elements) < get_tlb_entries()){
            element -> reference = current_time_in_milliseconds();
            list_add(tlb_elements, element);
        }else{
            bool _same_reference(void* tlb_element, void* other_element){
                return ((t_tlb_element*) tlb_element) -> reference <= ((t_tlb_element*) other_element) -> reference;
            }
            list_sort(tlb_elements, _same_reference);

            element -> reference = current_time_in_milliseconds();

            list_replace_and_destroy_element(tlb_elements, 0, element, free);
        }
    }
}

void lru_tlb_replacement(t_tlb_element* element){

    bool _page_already_in_tlb(void* tlb_element){
        return ((t_tlb_element*) tlb_element) -> page_number == element -> page_number;
    }

    if(!list_any_satisfy(tlb_elements, _page_already_in_tlb)){
        if(list_size(tlb_elements) < get_tlb_entries()){
            element -> reference = current_time_in_milliseconds();
            list_add(tlb_elements, element);
        }else{
            bool _same_reference(void* tlb_element, void* other_element){
                return ((t_tlb_element*) tlb_element) -> reference <= ((t_tlb_element*) other_element) -> reference;
            }
            list_sort(tlb_elements, _same_reference);

            element -> reference = current_time_in_milliseconds();

            list_replace_and_destroy_element(tlb_elements, 0, element, free);
        }
    }else{
        t_tlb_element* referenced_element = list_find(tlb_elements, _page_already_in_tlb);
        referenced_element -> reference = current_time_in_milliseconds();
    }
}

void add_new_element_to_tlb(uint32_t page_number, uint32_t frame_number){
    t_tlb_element* tlb_element = safe_malloc(sizeof(t_tlb_element));
    tlb_element -> page_number = page_number;
    tlb_element -> frame_number = frame_number;

    if(strcmp(get_tlb_replacement(), "FIFO") == 0){
        fifo_tlb_replacement(tlb_element);
    }else{
        lru_tlb_replacement(tlb_element);
    }
}


void initialize_tlb(){
    tlb_elements = safe_malloc(sizeof(t_tlb_element)*get_tlb_entries());

    log_tlb_succesfully_created();
}


void free_tlb(){
    list_destroy_and_destroy_elements(tlb_elements, free);
}