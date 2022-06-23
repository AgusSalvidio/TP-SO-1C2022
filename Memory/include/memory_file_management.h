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

void write_in_file(char* swap_file_path, uint32_t frame_number,uint32_t content);
uint32_t read_from_file(char* swap_file_path,uint32_t page_id);
void create_swap_file(char* file_path,uint32_t process_size);
void delete_file_from(char* swap_file_path);
void initialize_swap_file_for(uint32_t pid,uint32_t process_size);
char* swap_file_path_for(uint32_t pid);

#endif //KISS_MEMORY_FILE_MANAGEMENT_H
