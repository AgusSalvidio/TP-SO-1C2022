#ifndef KISS_MEMORY_REPLACEMENT_ALGORITHMS_H
#define KISS_MEMORY_REPLACEMENT_ALGORITHMS_H
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
    char* algorithm;
    t_list* process_context_collection;
}t_process_context_manager;

typedef struct {
    uint32_t pid;
    char* swap_file_path;
    uint32_t last_page_index_swapped;
    t_list* page_number_collection;
}t_process_context;

uint32_t loaded_page_quantity_of(uint32_t pid);
bool can_swap_page_for(uint32_t pid);
void initialize_swap_page_procedure(t_page* selected_page, t_process_context* process_context);
void swap_page_procedure(t_page* selected_page, uint32_t pid);
void initialize_memory_replacement_algorithm();
void new_process_context_for(uint32_t pid);
void suspend_process(uint32_t pid);
void finalize_process(uint32_t pid);
char* swap_file_path_for(uint32_t pid);



#endif //KISS_MEMORY_REPLACEMENT_ALGORITHMS_H
