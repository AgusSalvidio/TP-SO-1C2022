#include <memory_manager.h>
#include "../../Utils/include/common_structures.h"
#include "memory_configuration_manager.h"

t_main_memory* MAIN_MEMORY;
t_memory_table_manager* MEMORY_TABLE_MANAGER;

void initialize_memory_table_manager(){
    MEMORY_TABLE_MANAGER = safe_malloc(sizeof(t_memory_table_manager));
    MEMORY_TABLE_MANAGER->first_level_table_collection  = list_create();
    MEMORY_TABLE_MANAGER->second_level_table_collection = list_create();
    MEMORY_TABLE_MANAGER->last_first_level_table_id_assigned  = 0;
    MEMORY_TABLE_MANAGER->last_second_level_table_id_assigned = 0;
}

void initialize_memory_manager(){
    MAIN_MEMORY = safe_malloc(sizeof(t_main_memory));
    initialize_memory_table_manager();

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
void initialize_first_level_table_for(uint32_t pid){
    t_first_level_table* first_level_table = safe_malloc(sizeof(t_first_level_table));
    first_level_table->pid = pid;
    first_level_table->id = MEMORY_TABLE_MANAGER->last_first_level_table_id_assigned;
    increment_value(&(MEMORY_TABLE_MANAGER->last_first_level_table_id_assigned));
    first_level_table->second_level_table_id_collection = list_create();
}
void initialize_second_level_table_for(uint32_t pid,uint32_t page_quantity){

    for (uint32_t i = 0; i < entries_per_table_getter() ; ++i) {

    }

    t_second_level_table* second_level_table = safe_malloc(sizeof(t_second_level_table));
    second_level_table->id = MEMORY_TABLE_MANAGER->last_second_level_table_id_assigned;
    increment_value(&(MEMORY_TABLE_MANAGER->last_second_level_table_id_assigned));
    second_level_table->last_page_id_assigned = 0;
    second_level_table->pages_per_row = list_create();




}

void initialize_new_process_identified_and_sized_as(uint32_t pid, uint32_t process_page_quantity){

        initialize_first_level_table_for(pid);
        initialize_second_level_table_for(pid,process_page_quantity);

}

