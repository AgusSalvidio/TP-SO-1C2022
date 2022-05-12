#ifndef KISS_MEMORY_FILE_MANAGEMENT_H
#define KISS_MEMORY_FILE_MANAGEMENT_H

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

t_log* init_logger(void);
t_config* init_config(void);
void init_global_variables(void);

void initialize_memory(void);
void save_value_in_memory(uint32_t address, uint32_t value,uint32_t pid);
uint32_t read_value_in_memory(uint32_t address,uint32_t pid);
void initialize_memory_file_management();
void create_swap_file(uint32_t pid);
char* generate_path_file(uint32_t pid);
void save_content_in_swap_file(uint32_t address,uint32_t size,uint32_t pid);
void load_file_into_memory(uint32_t pid,uint32_t address,uint32_t size);


#endif //KISS_MEMORY_FILE_MANAGEMENT_H
