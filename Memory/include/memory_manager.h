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
}t_main_memory;


void initialize_memory_manager();
void initialize_new_process_identified_and_sized_as(uint32_t pid, uint32_t process_page_quantity);
uint32_t table_index_for(uint32_t pid);
void increase_value_by(uint32_t *value,uint32_t increase_value);
void increment_value(uint32_t *value);
uint32_t second_level_table_index_at(uint32_t index, uint32_t entry);
uint32_t frame_at(uint32_t page, uint32_t entry);

#endif //KISS_MEMORY_MANAGER_H
