//
// Created by utn_so on 24/04/22.
//

#ifndef MEMORY_MEMORY_MAIN_H
#define MEMORY_MEMORY_MAIN_H
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
//VARIABLES GLOBALES
void* MEMORY_BLOCK;
t_log* LOGGER;
t_config* CONFIG;
char* PATH_SWAP_BASE;
FILE* SWAP_FILE;
void* MEMORY_BLOCK_SECONDARY;
//VARIABLES GLOBALES


t_log* init_logger(void);
t_config* init_config(void);
void init_global_variables(void);



void init_memory(void);
void save_value_in_memory(uint32_t direction, uint32_t value,uint32_t pid);
uint32_t read_value_in_memory(uint32_t direction,uint32_t pid);

void create_swap_file(uint32_t pid);
char* generate_path_file(uint32_t pid);
void save_content_in_swap_file(uint32_t direction,uint32_t size,uint32_t pid);
void load_file_into_memory(uint32_t pid,uint32_t direction,uint32_t size);

#endif //MEMORY_MEMORY_MAIN_H
