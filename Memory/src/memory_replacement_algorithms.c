#include <memory_manager.h>
#include "../../Utils/include/common_structures.h"
#include "memory_replacement_algorithms.h"
#include "memory_configuration_manager.h"
#include "memory_file_management.h"
#include "../../Utils/include/pthread_wrapper.h"

t_process_context_manager* PROCESS_CONTEXT_MANAGER;
pthread_mutex_t mutex_process;

void initialize_process_context_manager(){
    PROCESS_CONTEXT_MANAGER = safe_malloc(sizeof(t_process_context_manager));
    PROCESS_CONTEXT_MANAGER->algorithm = algorithm();
    PROCESS_CONTEXT_MANAGER->process_context_collection = list_create();
    safe_mutex_initialize(&mutex_process);
}

char* swap_file_path_for(uint32_t pid){

    return string_from_format("%s%d.swap",swap_path(),pid);
}

void new_process_context_for(uint32_t pid){
    t_process_context* process_context = safe_malloc(sizeof(t_process_context));
    process_context->pid = pid;
    process_context->last_page_index_swapped = 0;
    process_context->swap_file_path = swap_file_path_for(pid);
    process_context->page_number_collection = list_create();

    list_add(PROCESS_CONTEXT_MANAGER->process_context_collection,process_context);
}

void initialize_memory_replacement_algorithm(){
    initialize_process_context_manager();
}

t_process_context* process_context_for(uint32_t pid){

    bool _pid_equals(void *process_context) {
        t_process_context *cast_process_context = (t_process_context *) process_context;
        return (cast_process_context->pid) == (pid);}

    return list_find(PROCESS_CONTEXT_MANAGER->process_context_collection,_pid_equals);
}

uint32_t loaded_page_quantity_of(uint32_t pid){

   return list_size(process_context_for(pid)->page_number_collection);
}

bool can_swap_page_for(uint32_t pid){
    return !list_is_empty(process_context_for(pid)->page_number_collection);
}

bool has_to_restart_clock_position(t_process_context* process_context){
    return (list_size(process_context->page_number_collection) == (process_context->last_page_index_swapped + 1));
}

bool can_page_be_swapped_at(t_process_context* process_context, uint32_t step){

    uint32_t page_id = list_get(process_context->page_number_collection,process_context->last_page_index_swapped);
    t_page* selected_page_to_swap = page_in_pid(process_context->pid,page_id);


    if(strcmp(PROCESS_CONTEXT_MANAGER->algorithm,"CLOCK") == 0){
        return !selected_page_to_swap->use_bit;
    }
    else
    {
        if(step == 1)
            return (selected_page_to_swap->use_bit == 0 && selected_page_to_swap->modified_bit == 0);
        else
            return (selected_page_to_swap->use_bit == 0 && selected_page_to_swap->modified_bit == 1);
    }

}

void update_page_bits(t_process_context *process_context){

    uint32_t page_id = list_get(process_context->page_number_collection,process_context->last_page_index_swapped);

    t_page* selected_page_to_swap = page_in_pid(process_context->pid,page_id);

    selected_page_to_swap ->use_bit = 0;

}

void save_content_to_file_for(t_process_context* process_context,t_page* victim_page){

    uint32_t victim_frame = victim_page->frame;
    uint32_t page_size = page_size_getter();
    uint32_t victim_page_content = read_value_at(victim_frame,page_size);

    write_in_file(process_context->swap_file_path,victim_frame,victim_page_content);

}

void load_content_to_memory_for(t_process_context* process_context,t_page* selected_page, uint32_t frame){

    uint32_t content_to_load = read_from_file(process_context->swap_file_path,selected_page->id);
    load_page_in_memory_at(frame,content_to_load);
    update_page_when_loaded(selected_page,frame);

}


void initialize_swap_page_procedure(t_page* selected_page, t_process_context* process_context){

    uint32_t pid = process_context->pid;
    uint32_t last_page_index = process_context->last_page_index_swapped;
    uint32_t victim_page_id = list_get(process_context->page_number_collection,last_page_index);
    t_page* victim_page = page_in_pid(pid,victim_page_id);

    safe_mutex_lock(&mutex_process);

    save_content_to_file_for(process_context,victim_page);
    list_replace(process_context_for(pid)->page_number_collection,last_page_index , selected_page->id);

    load_content_to_memory_for(process_context,selected_page,victim_page->frame);

    safe_mutex_unlock(&mutex_process);

}

void clock_algorithm(t_process_context* process_context,t_page* page_requested){

    bool end_search = false;
    uint32_t step = 1;
    while(!end_search){

        if(can_page_be_swapped_at(process_context, step)){
            initialize_swap_page_procedure(page_requested, process_context);
            if(has_to_restart_clock_position(process_context))
                process_context->last_page_index_swapped = 0;
            else
                increment_value(&(process_context->last_page_index_swapped));
            end_search = true;
        }
        else{
            if(has_to_restart_clock_position(process_context))
                process_context->last_page_index_swapped = 0;
            else
                increment_value(&(process_context->last_page_index_swapped));
            update_page_bits(process_context);
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
                process_context->last_page_index_swapped = 0;
            }
            else
                increment_value(&(process_context->last_page_index_swapped));
            end_search = true;
        }
        else{
            if(has_to_restart_clock_position(process_context)){
                process_context->last_page_index_swapped = 0;
                if(step == 1)
                    step = 2;
                else
                    step = 1;
            }
            else{
                if(step != 1)
                    update_page_bits(process_context);
                increment_value(&(process_context->last_page_index_swapped));
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

    t_list* page_id_collection = process_context->page_number_collection;

    for (int i = 0; i < list_size(page_id_collection) ; ++i) {
        t_page* page = list_get(page_id_collection,i);
        save_content_to_file_for(process_context,page);
        page->presence_bit = 0;
    }

    free_main_memory_frames(page_id_collection);
    list_clean(page_id_collection);

}
void finalize_process(uint32_t pid){
    suspend_process(pid);
    delete_file_from(swap_file_path_for(pid));
}
