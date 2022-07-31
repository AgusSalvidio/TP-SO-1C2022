#include "../include/memory_file_management.h"
#include "memory_configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "memory_logs_manager.h"


char* swap_file_path_for(uint32_t pid){

    return string_from_format("%s%d.swap",swap_path(),pid);
}

void initialize_swap_file_for(uint32_t pid, uint32_t process_size){

    char* swap_file_path = swap_file_path_for(pid);

    create_swap_file(swap_file_path, process_size);

    log_swap_file_for_process_was_successfully_initialized(pid,swap_file_path);

    consider_as_garbage(swap_file_path,free);

}

void write_in_file(char* swap_file_path, uint32_t page_id, uint32_t content, uint32_t offset){

    FILE* file_pointer = fopen(swap_file_path, "r+");
    char* transformed_content = string_itoa(content);

    uint32_t initial_position = page_id * PAGE_SIZE + offset;

    fseek(file_pointer, sizeof(char)*initial_position , SEEK_SET);

    fprintf(file_pointer, transformed_content);

    consider_as_garbage(transformed_content, free);
    fclose(file_pointer);
}

uint32_t read_from_file(char* swap_file_path,uint32_t page_id,uint32_t offset){

    FILE* file_pointer = fopen(swap_file_path, "r");

    uint32_t initial_position = page_id * PAGE_SIZE + offset;

    char* content_to_return = safe_malloc(PAGE_SIZE);

    fseek(file_pointer, initial_position, SEEK_SET);
    fgets(content_to_return, sizeof(uint32_t) + 1, file_pointer);

    consider_as_garbage(content_to_return, free);
    fclose(file_pointer);

    return atoi(content_to_return);
}

void create_swap_file(char* file_path,uint32_t process_size){

    FILE* file_pointer = fopen(file_path, "w");
    uint32_t file_descriptor = fileno(file_pointer);
    ftruncate(file_descriptor, process_size);

    fputc('\0', file_pointer);

    fclose(file_pointer);

}

void delete_file_from(char* swap_file_path){

    if(remove(swap_file_path) == 0)
        log_swap_file_delete_procedure_description(swap_file_path,"fue eliminado correctamente.");
    else
        log_swap_file_delete_procedure_description(swap_file_path,"no pudo ser eliminado debido a un error.");

}
