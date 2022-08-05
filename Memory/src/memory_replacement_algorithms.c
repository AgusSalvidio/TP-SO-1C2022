#include <memory_manager.h>
#include "../../Utils/include/common_structures.h"
#include "memory_replacement_algorithms.h"
#include "memory_configuration_manager.h"
#include "memory_file_management.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "memory_logs_manager.h"
#include "../../Utils/include/garbage_collector.h"

t_process_context_manager* PROCESS_CONTEXT_MANAGER;
pthread_mutex_t mutex_process;

void wait_swap_delay_time(){
    uint32_t delay_time_in_microseconds = swap_time()*1000;
    log_swap_delay_time_has_started(swap_time());
    usleep(delay_time_in_microseconds);
    log_swap_delay_time_has_ended();
}

void initialize_process_context_manager(){
    PROCESS_CONTEXT_MANAGER = safe_malloc(sizeof(t_process_context_manager));
    PROCESS_CONTEXT_MANAGER->algorithm = algorithm();
    PROCESS_CONTEXT_MANAGER->process_context_collection = list_create();
    safe_mutex_initialize(&mutex_process);

    log_process_context_manager_was_successfully_initialized();

}

void add_frame_related_to_page_to(t_process_context* process_context,t_page* page){

    t_frame_related_to_page_id* frame_related_to_page = safe_malloc(sizeof(t_frame_related_to_page_id));
    frame_related_to_page->frame = page->frame;
    frame_related_to_page->page_id = page->id;

    list_add(process_context->frame_related_to_page_id_collection,frame_related_to_page);

    log_frame_related_to_page_was_added_to_process_context_successfully(frame_related_to_page->frame, frame_related_to_page->page_id);

    consider_as_garbage(frame_related_to_page, free);
}

void update_page_modified_bit(t_page* page,uint32_t bit){
    page->modified_bit = bit;
}

void update_page_use_bit(t_page* page,uint32_t bit){
    page->use_bit = bit;
}

void update_page_presence_bit(t_page* page,uint32_t bit){
    page->presence_bit = bit;
}

void update_page_presence_bit_when_unload(t_page* page){
    update_page_presence_bit(page,0);
}

void update_page_bits_when_read(t_page* page){
    update_page_use_bit(page,1);

}

void update_page_bits_when_written(t_page* page){
    update_page_use_bit(page,1);
    update_page_modified_bit(page,1);

}

void new_process_context_for(uint32_t pid){
    t_process_context* process_context = safe_malloc(sizeof(t_process_context));
    process_context->pid = pid;
    process_context->clock_pointer = 0;
    process_context->swap_file_path = swap_file_path_for(pid);
    process_context->frame_related_to_page_id_collection = list_create();

    list_add(PROCESS_CONTEXT_MANAGER->process_context_collection,process_context);

    log_process_context_was_successfully_initialized(pid);
}

t_process_context* process_context_for(uint32_t pid){

    bool _pid_equals(void *process_context) {
        t_process_context *cast_process_context = (t_process_context *) process_context;
        return (cast_process_context->pid) == (pid);}

    return list_find(PROCESS_CONTEXT_MANAGER->process_context_collection,_pid_equals);
}

uint32_t loaded_page_quantity_of(uint32_t pid){

   return list_size(process_context_for(pid)->frame_related_to_page_id_collection);
}

bool can_swap_page_for(uint32_t pid){
    if(list_is_empty(process_context_for(pid)->frame_related_to_page_id_collection))
        return 0;
    return 1;
}

bool has_to_restart_clock_position(t_process_context* process_context){
    return (list_size(process_context->frame_related_to_page_id_collection) == (process_context->clock_pointer + 1));
}

bool can_page_be_swapped_at(t_process_context* process_context, uint32_t step){

    t_frame_related_to_page_id*  frame_related_to_page = list_get(process_context->frame_related_to_page_id_collection, process_context->clock_pointer);

    t_page* selected_page_to_swap = page_in_pid(process_context->pid,frame_related_to_page->page_id);

    if(strcmp(PROCESS_CONTEXT_MANAGER->algorithm,"CLOCK") == 0){
        uint32_t use_bit = selected_page_to_swap->use_bit;
        if (use_bit == 0)
            return 1;
        else
            return 0;
    }
    else
    {
        if(step == 1)
            return (selected_page_to_swap->use_bit == 0 && selected_page_to_swap->modified_bit == 0);
        else
            return (selected_page_to_swap->use_bit == 0 && selected_page_to_swap->modified_bit == 1);
    }

}

void update_use_bit_from_page_in(t_process_context *process_context){

    t_frame_related_to_page_id* frame_related_to_page = list_get(process_context->frame_related_to_page_id_collection, process_context->clock_pointer);

    t_page* selected_page_to_swap = page_in_pid(process_context->pid,frame_related_to_page->page_id);

    update_page_use_bit(selected_page_to_swap,0);

}

void save_content_to_file_for(t_process_context* process_context,t_page* victim_page){

    uint32_t victim_frame = victim_page->frame;
    uint32_t victim_page_number = victim_page->id;
    uint32_t victim_page_content;

    FILE* file_pointer = fopen(process_context->swap_file_path, "r+");

    for (int offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t)) {
        victim_page_content = read_value_at(victim_frame,offset);
        write_in_file(file_pointer,victim_page_number,victim_page_content,offset);
    }
    fclose(file_pointer);
    update_page_presence_bit_when_unload(victim_page);
}

void update_page_bits_when_loaded_in_main_memory(t_page* page, uint32_t frame){
    page->frame = frame;
    page->presence_bit = 1;
    page->use_bit = 0;                  //Only used when read or write/copy are called
    page->modified_bit = 0;
}

t_page* page_located_in(uint32_t frame){

    bool _frame_equals(void *frame_related_to_page_id) {
        t_frame_related_to_page_id *cast_frame_related_to_page_id = (t_frame_related_to_page_id *) frame_related_to_page_id;

        return cast_frame_related_to_page_id->frame == frame;}

    bool _index_equals(void *process_context) {
        t_process_context *cast_process_context = (t_process_context *) process_context;
        return list_find(cast_process_context->frame_related_to_page_id_collection,_frame_equals);}

    t_process_context* process_context = list_find(PROCESS_CONTEXT_MANAGER->process_context_collection,_index_equals);

    t_frame_related_to_page_id* frame_related_to_page = list_find(process_context->frame_related_to_page_id_collection,_frame_equals);

    return page_in_pid(process_context->pid,frame_related_to_page->page_id);
}

void load_content_to_memory_for(t_process_context* process_context,t_page* selected_page, uint32_t frame){

    uint32_t content_to_load;
    safe_mutex_lock(&mutex_process);

    FILE* file_pointer = fopen(process_context->swap_file_path, "r");

    for (int offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t)) {
        content_to_load = read_from_file(file_pointer,selected_page->id,offset);
        write_value_at(frame, offset,content_to_load);
    }

    update_page_bits_when_loaded_in_main_memory(selected_page, frame);               //Update bits and frame for pages_per_row
    fclose(file_pointer);
    safe_mutex_unlock(&mutex_process);


}

t_frame_related_to_page_id* frame_related_to_page_using(uint32_t frame, uint32_t page_id){

    t_frame_related_to_page_id* frame_related_to_page = safe_malloc(sizeof(t_frame_related_to_page_id));

    frame_related_to_page->frame = frame;
    frame_related_to_page->page_id = page_id;

    return frame_related_to_page;

}

void initialize_swap_page_procedure(t_page* selected_page, t_process_context* process_context){

    uint32_t pid = process_context->pid;
    uint32_t last_page_index = process_context->clock_pointer;

    t_frame_related_to_page_id* frame_related_to_victim_page = list_get(process_context->frame_related_to_page_id_collection, last_page_index);

    t_page* victim_page = page_in_pid(pid, frame_related_to_victim_page->page_id);

    safe_mutex_lock(&mutex_process);
    //Improve performance by saving content to files only for pages that were modified.
    if(victim_page->modified_bit == 1)
        save_content_to_file_for(process_context,victim_page);
    else
        update_page_presence_bit_when_unload(victim_page);
    safe_mutex_unlock(&mutex_process);

    t_frame_related_to_page_id* frame_related_to_page = frame_related_to_page_using(frame_related_to_victim_page->frame,selected_page->id);
    list_replace(process_context_for(pid)->frame_related_to_page_id_collection, last_page_index , frame_related_to_page);
    load_content_to_memory_for(process_context,selected_page,victim_page->frame);

    consider_as_garbage(frame_related_to_page, free);
    wait_swap_delay_time();
    log_swap_procedure_was_successful(pid,victim_page->id,selected_page->id);

}

void clock_algorithm(t_process_context* process_context,t_page* page_requested){

    bool end_search = false;
    uint32_t step = 1;
    while(!end_search){

        if(can_page_be_swapped_at(process_context, step)){
            initialize_swap_page_procedure(page_requested, process_context);
            if(has_to_restart_clock_position(process_context))
                process_context->clock_pointer = 0;
            else
                increment_value(&(process_context->clock_pointer));
            end_search = true;
        }
        else{
            update_use_bit_from_page_in(process_context);
            if(has_to_restart_clock_position(process_context))
                process_context->clock_pointer = 0;
            else
                increment_value(&(process_context->clock_pointer));
        }
    }
}
void enhanced_clock_algorithm(t_process_context* process_context,t_page* page_requested){

    //Needs to be refactored to be more clean.

    bool end_search = false;
    uint32_t step = 1;

    while(!end_search){

        if(can_page_be_swapped_at(process_context, step)){
            initialize_swap_page_procedure(page_requested, process_context);
            if(has_to_restart_clock_position(process_context)) {
                process_context->clock_pointer = 0;
            }
            else
                increment_value(&(process_context->clock_pointer));
            end_search = true;
        }
        else{
            if(has_to_restart_clock_position(process_context)){
                if(step == 1)
                    step = 2;
                else{
                    step = 1;
                    update_use_bit_from_page_in(process_context);}
                process_context->clock_pointer = 0;
            }
            else{
                if(step == 2)
                    update_use_bit_from_page_in(process_context);
                increment_value(&(process_context->clock_pointer));
            }
        }


    }


}

void swap_page_procedure(t_page* selected_page, uint32_t pid){

   t_process_context* process_context = process_context_for(pid);

    if(strcmp(PROCESS_CONTEXT_MANAGER->algorithm,"CLOCK") == 0)
        clock_algorithm(process_context,selected_page);
    else
        enhanced_clock_algorithm(process_context,selected_page);
}

void suspend_process(uint32_t pid){

    t_process_context * process_context = process_context_for(pid);

    t_list* frame_related_to_page_id_collection = process_context->frame_related_to_page_id_collection;

    for (int i = 0; i < list_size(frame_related_to_page_id_collection) ; ++i) {

        t_frame_related_to_page_id* frame_related_to_page_id =  list_get(frame_related_to_page_id_collection, i);

        t_page* page = page_in_pid(pid,frame_related_to_page_id->page_id);

        //To improve performance, when a page was not modified, then dont save content, because this will waste I/O time
        if(page->modified_bit == 1)
            save_content_to_file_for(process_context,page);
        else
            update_page_presence_bit_when_unload(page);
    }

    free_main_memory_frames(frame_related_to_page_id_collection);
    list_clean(frame_related_to_page_id_collection);

}
void finalize_process(uint32_t pid){
    suspend_process(pid);
    char* swap_file_path = swap_file_path_for(pid);
    delete_file_from(swap_file_path);
    consider_as_garbage(swap_file_path, free);
}

void free_memory_replacement_algorithms(){

    list_destroy_and_destroy_elements(PROCESS_CONTEXT_MANAGER->process_context_collection,free);
    free(PROCESS_CONTEXT_MANAGER);

}
