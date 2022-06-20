#ifndef KISS_MEMORY_MANAGER_H
#define KISS_MEMORY_MANAGER_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint-gcc.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<string.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<commons/collections/list.h>
#include<pthread.h>
#include "../../Utils/include/common_structures.h"

typedef struct {
    t_list* first_level_table_collection;
    t_list* second_level_table_collection;
    uint32_t last_first_level_table_id_assigned;
    uint32_t last_second_level_table_id_assigned;
}t_memory_table_manager;

typedef struct {
    uint32_t pid;
    uint32_t id;
    t_list* second_level_table_id_collection;
}t_first_level_table;

typedef struct {
    uint32_t id;
    uint32_t last_page_id_assigned;
    t_list* pages_per_row;
}t_second_level_table;

typedef struct {
    uint32_t id;
    uint32_t frame;
    uint8_t presence_bit;
    uint8_t use_bit;
    uint8_t modified_bit;
}t_page;

typedef struct {
    uint32_t size;
    uint32_t frame_array[16]; //hacer cuenta de cantidad
    t_list* available_frames;
}t_main_memory;


void initialize_memory_manager();
void initialize_new_process(uint32_t pid, uint32_t process_page_quantity,uint32_t process_size);

uint32_t table_index_for(uint32_t pid);
void increase_value_by(uint32_t *value,uint32_t increase_value);
void increment_value(uint32_t *value);
uint32_t second_level_table_index_at(uint32_t index, uint32_t entry);
uint32_t pid_for(uint32_t second_level_table_index);
uint32_t frame_at(uint32_t page, uint32_t entry);
t_page* page_at(t_second_level_table* second_level_table,uint32_t entry);
t_page* page_in_pid(uint32_t pid, uint32_t page_id);
bool is_page_loaded_in_main_memory(t_page* page);
t_second_level_table* second_level_table_for(uint32_t index);
uint32_t read_value_from(t_physical_address* physical_address);
uint32_t write_value_on(t_physical_address* physical_address,uint32_t value_to_write);
uint32_t read_value_at(uint32_t frame, uint32_t offset);
uint32_t write_value_at(uint32_t frame,uint32_t offset,uint32_t value_to_write);
void load_page_in_memory(t_page* page, uint32_t pid);
bool can_memory_load_another_page_for(uint32_t pid);
void free_main_memory_frames(t_list* page_id_collection);
#endif //KISS_MEMORY_MANAGER_H
