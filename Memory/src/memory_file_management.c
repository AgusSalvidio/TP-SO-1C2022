#include "../include/memory_file_management.h"
#include "memory_configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "memory_logs_manager.h"
#include "memory_replacement_algorithms.h"

void initialize_swap_file_for(uint32_t pid, uint32_t process_size){
    create_swap_file(swap_file_path_for(pid), process_size);
}

void write_in_file(char* swap_file_path, uint32_t frame_number,uint32_t content){

    FILE* file_pointer = fopen(swap_file_path, "r+");
    uint32_t offset_position = frame_number * page_size_getter();
    char* transformed_content = string_itoa(content);

    fseek(file_pointer, sizeof(uint32_t)*offset_position, SEEK_SET);

    fprintf(file_pointer, transformed_content);

    consider_as_garbage(transformed_content, free);
    fclose(file_pointer);
}

uint32_t read_from_file(char* swap_file_path,uint32_t page_id){

    FILE* file_pointer = fopen(swap_file_path, "r");

    uint32_t initial_position = page_id * page_size_getter();
    uint32_t displacement = page_size_getter();

    char* content_to_return = safe_malloc(displacement);

    fseek(file_pointer, sizeof(char)*initial_position, SEEK_SET);
    fgets(content_to_return, displacement+1, file_pointer);

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
