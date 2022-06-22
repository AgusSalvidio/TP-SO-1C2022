#include <memory_manager.h>
#include "../../Utils/include/common_structures.h"
#include "memory_configuration_manager.h"
#include "memory_replacement_algorithms.h"
#include "memory_request_handler.h"
#include "memory_file_management.h"

t_main_memory* MAIN_MEMORY;
t_memory_table_manager* MEMORY_TABLE_MANAGER;

uint32_t frame_available(){
    uint32_t frame = list_get(MAIN_MEMORY->available_frames,0);
    list_remove(MAIN_MEMORY->available_frames,0);
    return frame;
}

void initialize_memory_table_manager(){
    MEMORY_TABLE_MANAGER = safe_malloc(sizeof(t_memory_table_manager));
    MEMORY_TABLE_MANAGER->first_level_table_collection  = list_create();
    MEMORY_TABLE_MANAGER->second_level_table_collection = list_create();
    MEMORY_TABLE_MANAGER->last_first_level_table_id_assigned  = 0;
    MEMORY_TABLE_MANAGER->last_second_level_table_id_assigned = 0;
}

void initialize_main_memory(){

    uint32_t frame_quantity = quantity_memory_frames();

    MAIN_MEMORY = safe_malloc(sizeof(t_main_memory));
    MAIN_MEMORY->buffer = safe_malloc(sizeof(uint32_t)*frame_quantity);
    MAIN_MEMORY->available_frames = list_create();

    for (int i = 0; i < frame_quantity ; ++i)
        list_add(MAIN_MEMORY->available_frames,i);
}

void initialize_memory_manager(){
    MAIN_MEMORY = safe_malloc(sizeof(t_main_memory));
    initialize_main_memory();
    initialize_memory_table_manager();
    initialize_memory_replacement_algorithm();
}
void increase_value_by(uint32_t *value,uint32_t increase_value){
    (*value) += increase_value;
}
void increment_value(uint32_t *value){
    increase_value_by(value,7);
}
t_first_level_table* first_level_table_for(uint32_t pid){

    bool _pid_equals(void *first_level_table) {
        t_first_level_table *cast_first_level_table = (t_first_level_table *) first_level_table;
        return (cast_first_level_table->pid) == (pid);
    }

    t_first_level_table* first_level_table = list_find(MEMORY_TABLE_MANAGER->first_level_table_collection, _pid_equals);

    return first_level_table;
}

uint32_t table_index_for(uint32_t pid){
    //Return the index table for the pid received. Check handle null case.
    return (first_level_table_for(pid))->id;
}

uint32_t pid_for(uint32_t second_level_table_index){

    bool _index_equals(uint32_t second_table_id) {
        return second_table_id == second_level_table_index;
    }

    bool _table_satisfies(void *first_level_table) {
        t_first_level_table *cast_first_level_table = (t_first_level_table *) first_level_table;
        return list_any_satisfy(cast_first_level_table->second_level_table_id_collection,_index_equals);
    }

    t_first_level_table* first_level_table =  list_find(MEMORY_TABLE_MANAGER->first_level_table_collection, _table_satisfies);

    return first_level_table->pid;
}

uint32_t second_level_table_index_at(uint32_t index, uint32_t entry){

    bool _index_equals(void *first_level_table) {
        t_first_level_table *cast_first_level_table = (t_first_level_table *) first_level_table;
        return (cast_first_level_table->id) == (index);
    }

    t_first_level_table* first_level_table = list_find(MEMORY_TABLE_MANAGER->first_level_table_collection, _index_equals);

    return list_get(first_level_table->second_level_table_id_collection,entry);

}

bool are_available_frames_in_memory(){

   return list_is_empty(MAIN_MEMORY->available_frames);
}

bool can_memory_load_another_page_for(uint32_t pid){

    if(loaded_page_quantity_of(pid) < frames_per_process_getter())
        return are_available_frames_in_memory();
    else
        return false;
}

bool is_page_loaded_in_main_memory(t_page* page){

    if(page->presence_bit == 1)
        return true;
    else
        return false;
}

t_page* page_in_pid(uint32_t pid, uint32_t page_id){

    t_first_level_table* first_level_table = first_level_table_for(pid);

    bool _page_id_equals(void *page) {
        t_page* cast_page = (t_page *) page;
        return cast_page->id == page_id;
    }

    bool _index_equals(void *second_level_table_id) {
        t_second_level_table* second_level_table = second_level_table_for(second_level_table_id);
        return list_any_satisfy(second_level_table->pages_per_row,_page_id_equals);
    }

    uint32_t second_level_table_index = list_find(first_level_table->second_level_table_id_collection,_index_equals);

    t_second_level_table* second_level_table = second_level_table_for(second_level_table_index);

    return list_find(second_level_table->pages_per_row,_page_id_equals);
}

t_page* page_at(t_second_level_table* second_level_table,uint32_t entry){

    return list_get(second_level_table->pages_per_row,entry);

}

t_second_level_table* second_level_table_for(uint32_t index){
    bool _index_equals(void *second_level_table) {
        t_second_level_table *cast_second_level_table = (t_second_level_table *) second_level_table;
        return (cast_second_level_table->id) == (index);
    }

    return list_find(MEMORY_TABLE_MANAGER->second_level_table_collection, _index_equals);
}

uint32_t frame_at(uint32_t index, uint32_t entry){

    t_second_level_table * second_level_table = second_level_table_for(index);
    return (page_at(second_level_table,entry))->frame;
}

void free_main_memory_frames(t_list* page_id_collection){

    for (int i = 0; i < list_size(page_id_collection) ; ++i)
        list_add(MAIN_MEMORY->available_frames, list_get(page_id_collection,i));
}

void load_page_in_memory(t_page* page, uint32_t pid) {

    uint32_t frame = frame_available();

    uint32_t content = read_from_file(swap_file_path_for(pid),page->id);

    write_value_at(frame, sizeof(uint32_t),content);


}

uint32_t frame_parse_from(t_physical_address* physical_address){
    //Work in progress
}
uint32_t offset_parse_from(t_physical_address* physical_address){
    //Work in progress
}
uint32_t read_value_at(uint32_t frame, uint32_t offset){

    uint32_t value;
    memcpy(&value,(MAIN_MEMORY->buffer) + frame,offset* sizeof(uint32_t));
    return value;

}
void write_value_at(uint32_t frame,uint32_t offset,uint32_t value_to_write){

    memcpy((MAIN_MEMORY->buffer) + frame,&value_to_write,offset);

}
uint32_t read_value_from(t_physical_address* physical_address){

    uint32_t frame = frame_parse_from(physical_address);
    uint32_t offset = offset_parse_from(physical_address);

    return read_value_at(frame,offset);
}
void write_value_on(t_physical_address* physical_address,uint32_t value_to_write){

    uint32_t frame = frame_parse_from(physical_address);
    uint32_t offset = offset_parse_from(physical_address);
    write_value_at(frame,offset,value_to_write);
}

void initialize_first_level_table_for(uint32_t pid){
    t_first_level_table* first_level_table = safe_malloc(sizeof(t_first_level_table));
    first_level_table->pid = pid;
    first_level_table->id = MEMORY_TABLE_MANAGER->last_first_level_table_id_assigned;
    increment_value(&(MEMORY_TABLE_MANAGER->last_first_level_table_id_assigned));
    first_level_table->second_level_table_id_collection = list_create();
    list_add(MEMORY_TABLE_MANAGER->first_level_table_collection,first_level_table);
}

t_page* new_page_identified_by(uint32_t page_id){

    t_page* page = safe_malloc(sizeof(t_page));
    page->id = page_id;
    page->frame = 0;
    page->presence_bit = 0;
    page->use_bit = 0;
    page->modified_bit = 0;
}

void add_pages_to(t_second_level_table* second_level_table,uint32_t page_amount){

    for (uint32_t i = 0; i < page_amount ; ++i){
        list_add(second_level_table->pages_per_row, new_page_identified_by(second_level_table->last_page_id_assigned));
        increment_value(&(second_level_table->last_page_id_assigned));
    }

}

void register_second_level_table_using(t_first_level_table* first_level_table,uint32_t page_amount){

    t_second_level_table* second_level_table = safe_malloc(sizeof(t_second_level_table));
    second_level_table->id = MEMORY_TABLE_MANAGER->last_second_level_table_id_assigned;
    increment_value(&(MEMORY_TABLE_MANAGER->last_second_level_table_id_assigned));
    second_level_table->last_page_id_assigned = 0;
    second_level_table->pages_per_row = list_create();

    add_pages_to(second_level_table,page_amount);

    list_add(first_level_table->second_level_table_id_collection, second_level_table->id);
    list_add(MEMORY_TABLE_MANAGER->second_level_table_collection,second_level_table);

}

void initialize_second_level_table_for(uint32_t pid,uint32_t page_quantity){

    t_first_level_table * first_level_table = first_level_table_for(pid);

    uint32_t page_amount = entries_per_table_getter();

    uint32_t remnant = page_quantity % page_amount;
    uint32_t quotient = page_quantity / page_amount;

    if(remnant!=0 && quotient !=0){

        for (uint32_t i = 0; i < quotient ; ++i)
            register_second_level_table_using(first_level_table,page_amount);
        register_second_level_table_using(first_level_table,remnant);    //Register the second_level_table with no full amount of pages.
    }
    else
    if(quotient == 0 && remnant !=0)
        register_second_level_table_using(first_level_table,remnant);
    else
    {
        for (uint32_t i = 0; i < quotient ; ++i)
            register_second_level_table_using(first_level_table,page_amount);
    }

}

void initialize_new_process(uint32_t pid, uint32_t process_page_quantity,uint32_t process_size){

    initialize_first_level_table_for(pid);
    initialize_second_level_table_for(pid,process_page_quantity);
    initialize_swap_file_for(pid,process_size);
    new_process_context_for(pid);

}

