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

void load_page_in_memory(t_page* page, uint32_t pid){

    // 1er Paso: Encontrar la pagina victima para sacar de memoria usando el algoritmo.
    // 2do Paso: Una vez encontrada la misma, se procede a guardar lo que tenga en memoria principal y se guarda en el archivo.
    // 3er Paso: Cargar en memoria principal la pagina solicitada
    // 4to Paso: Devolver el valor o respuesta en funcion de la operacion.

}

bool is_page_loaded_in_main_memory(t_page* page){

    if(page->presence_bit == 1)
        return true;
    else
        return false;
}

t_page* page_at(t_second_level_table* second_level_table,uint32_t entry){

    t_page* selected_page = list_get(second_level_table->pages_per_row,entry);

    if(!is_page_loaded_in_main_memory(selected_page)){

        uint32_t pid = pid_for(second_level_table->id);
        load_page_in_memory(selected_page,pid);
    }
    return selected_page;
}

uint32_t frame_at(uint32_t index, uint32_t entry){

    bool _index_equals(void *second_level_table) {
        t_second_level_table *cast_second_level_table = (t_second_level_table *) second_level_table;
        return (cast_second_level_table->id) == (index);
    }

    t_second_level_table * second_level_table = list_find(MEMORY_TABLE_MANAGER->second_level_table_collection, _index_equals);
    return (page_at(second_level_table,entry))->frame;
}

uint32_t frame_parse_from(t_physical_address* physical_address){
    //Work in progress
}
uint32_t offset_parse_from(t_physical_address* physical_address){
    //Work in progress
}
uint32_t read_value_at(uint32_t frame, uint32_t offset){
    //Work in progress

}
uint32_t write_value_at(uint32_t frame,uint32_t offset,uint32_t value_to_write){

    //Work in progress
}
uint32_t read_value_from(t_physical_address* physical_address){

    uint32_t frame = frame_parse_from(physical_address);
    uint32_t offset = offset_parse_from(physical_address);

    return read_value_at(frame,offset);
}
uint32_t write_value_on(t_physical_address* physical_address,uint32_t value_to_write){

    uint32_t frame = frame_parse_from(physical_address);
    uint32_t offset = offset_parse_from(physical_address);

    return write_value_at(frame,offset,value_to_write);

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

void suspend_process(uint32_t pid){

}
void finalize_process(uint32_t pid){

}
void initialize_new_process(uint32_t pid, uint32_t process_page_quantity){

        initialize_first_level_table_for(pid);
        initialize_second_level_table_for(pid,process_page_quantity);

}

