#include <memory_manager.h>
#include "../../Utils/include/common_structures.h"
#include "memory_configuration_manager.h"
#include "memory_replacement_algorithms.h"
#include "memory_request_handler.h"
#include "memory_file_management.h"
#include "memory_logs_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/garbage_collector.h"

t_main_memory* MAIN_MEMORY;
t_memory_table_manager* MEMORY_TABLE_MANAGER;
pthread_mutex_t mutex;

#define MAIN_MEMORY_BUFFER (MAIN_MEMORY->buffer)

uint32_t frame_available(){
    uint32_t frame = list_get(MAIN_MEMORY->available_frames,0);
    list_remove(MAIN_MEMORY->available_frames,0);
    return frame;
}

void initialize_memory_table_manager(){
    MEMORY_TABLE_MANAGER = safe_malloc(sizeof(t_memory_table_manager));
    MEMORY_TABLE_MANAGER->first_level_table_collection  = list_create();
    MEMORY_TABLE_MANAGER->second_level_table_collection = list_create();
    MEMORY_TABLE_MANAGER->next_first_level_table_id_to_assign  = 0;
    MEMORY_TABLE_MANAGER->next_second_level_table_id_to_assign = 0;

    log_memory_table_manager_was_successfully_initialized();
}

void initialize_main_memory(){

    MAIN_MEMORY = safe_malloc(sizeof(t_main_memory));
    MAIN_MEMORY->buffer = safe_malloc(MEMORY_SIZE);
    MAIN_MEMORY->available_frames = list_create();

    for (int i = 0; i < MEMORY_FRAME_QUANTITY ; ++i)
        list_add(MAIN_MEMORY->available_frames,i);

    safe_mutex_initialize(&mutex);
    log_main_memory_was_successfully_initialized();

}

void initialize_memory_manager(){
    initialize_main_memory();
    initialize_memory_table_manager();
    initialize_process_context_manager();
}
void increase_value_by(uint32_t *value,uint32_t increase_value){
    (*value) += increase_value;
}
void increment_value(uint32_t *value){
    increase_value_by(value,1);
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
   if(list_is_empty(MAIN_MEMORY->available_frames))
       return false;
   else
       return true;
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

void free_main_memory_frames(t_list* frame_related_to_page_id_collection){

    for (int i = 0; i < list_size(frame_related_to_page_id_collection) ; ++i){

        t_frame_related_to_page_id* frame_related_to_page = list_get(frame_related_to_page_id_collection, i);

        list_add(MAIN_MEMORY->available_frames,frame_related_to_page->frame);
    }

    log_main_memory_frames_were_successfully_released();

}

void load_page_in_memory(t_page* page, uint32_t pid) {

    uint32_t frame = frame_available();
    uint32_t content;

    safe_mutex_lock(&mutex);

    page->frame = frame;                            //Update frame here because the process_context, copies the frame from the page and if it was suspended, it will have the old one
    add_frame_related_to_page_to(process_context_for(pid),page);

    FILE* file_pointer = fopen(swap_file_path_for(pid), "r");

    for (int offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t)) {
        content = read_from_file(file_pointer,page->id,offset);
        write_value_at(frame, offset,content);
    }

    update_page_bits_when_loaded_in_main_memory(page,frame);

    safe_mutex_unlock(&mutex);
    fclose(file_pointer);
    log_page_was_loaded_in_memory_successfully(pid,page);
}

uint32_t frame_parse_from(t_physical_address* physical_address){
    return physical_address->frame;
}
uint32_t offset_parse_from(t_physical_address* physical_address){
    return physical_address->offset;
}

uint32_t read_value_at(uint32_t frame, uint32_t offset){

    uint32_t page_size = page_size_getter();
    uint32_t value;
    memcpy(&value,MAIN_MEMORY_BUFFER + (frame * page_size) + offset,sizeof(uint32_t));

    log_memory_read_at(frame,offset);

    return value;

}
void write_value_at(uint32_t frame,uint32_t offset,uint32_t value_to_write){

    memcpy(MAIN_MEMORY_BUFFER + (frame * PAGE_SIZE) + offset,&value_to_write, sizeof(uint32_t));

    log_memory_write_at(frame,offset,value_to_write);


}
uint32_t read_value_from(t_physical_address* physical_address){

    uint32_t frame = frame_parse_from(physical_address);
    uint32_t offset = offset_parse_from(physical_address);
    uint32_t value = read_value_at(frame,offset);
    update_page_bits_when_read(page_located_in(frame));

    return value;
}
void write_value_on(t_physical_address* physical_address,uint32_t value_to_write){

    uint32_t frame = frame_parse_from(physical_address);
    uint32_t offset = offset_parse_from(physical_address);

    write_value_at(frame,offset,value_to_write);

    update_page_bits_when_written(page_located_in(frame));

}

void initialize_first_level_table_for(uint32_t pid){

    t_first_level_table* first_level_table = safe_malloc(sizeof(t_first_level_table));
    first_level_table->pid = pid;
    first_level_table->id = MEMORY_TABLE_MANAGER->next_first_level_table_id_to_assign;
    increment_value(&(MEMORY_TABLE_MANAGER->next_first_level_table_id_to_assign));
    first_level_table->second_level_table_id_collection = list_create();
    first_level_table->next_page_id_to_assign = 0;
    list_add(MEMORY_TABLE_MANAGER->first_level_table_collection,first_level_table);

    log_first_level_table_for_process_was_successfully_initialized(pid);

}

t_page* new_page_identified_by(uint32_t page_id){

    t_page* page = safe_malloc(sizeof(t_page));
    page->id = page_id;
    page->frame = 0;
    page->presence_bit = 0;
    page->use_bit = 0;
    page->modified_bit = 0;

    return page;
}

void add_pages_to(t_second_level_table* second_level_table,uint32_t page_amount, t_first_level_table* first_level_table){

    for (uint32_t i = 0; i < page_amount ; ++i){
        list_add(second_level_table->pages_per_row, new_page_identified_by(first_level_table->next_page_id_to_assign));
        increment_value(&(first_level_table->next_page_id_to_assign));
    }

}

void register_second_level_table_using(t_first_level_table* first_level_table,uint32_t page_amount){

    t_second_level_table* second_level_table = safe_malloc(sizeof(t_second_level_table));
    second_level_table->id = MEMORY_TABLE_MANAGER->next_second_level_table_id_to_assign;
    increment_value(&(MEMORY_TABLE_MANAGER->next_second_level_table_id_to_assign));
    second_level_table->pages_per_row = list_create();

    add_pages_to(second_level_table,page_amount,first_level_table);

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

    log_second_level_table_for_process_was_successfully_initialized(pid);
}

void initialize_new_process(uint32_t pid, uint32_t process_page_quantity,uint32_t process_size){

    initialize_first_level_table_for(pid);
    initialize_second_level_table_for(pid,process_page_quantity);
    initialize_swap_file_for(pid,process_size);
    new_process_context_for(pid);

}

