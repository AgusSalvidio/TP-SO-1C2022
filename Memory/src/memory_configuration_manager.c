#include "memory_configuration_manager.h"
#include "memory_logs_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <string.h>
#include <commons/string.h>
#include <stdint.h>

char* server_port;
uint32_t memory_size;
uint32_t page_size;
uint32_t entries_per_table;
uint32_t memory_delay_time;
char* replacement_algorithm;
uint32_t frames_per_process;
uint32_t swap_delay_time;
char* swap_file_path;

void initialize_memory_configuration_manager(){
    initialize_configuration_manager();
    server_port = config_get_string_at("PUERTO_ESCUCHA");
    memory_size = config_get_int_at("TAM_MEMORIA");
    page_size = config_get_int_at("TAM_PAGINA");
    entries_per_table = config_get_int_at("ENTRADAS_POR_TABLA");
    memory_delay_time = config_get_int_at("RETARDO_MEMORIA");
    replacement_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");
    frames_per_process = config_get_int_at("MARCOS_POR_PROCESO");
    swap_delay_time = config_get_int_at("RETARDO_SWAP");
    swap_file_path = config_get_string_at("PATH_SWAP");

}
char* port_getter(){
    return server_port;
}
uint32_t memory_size_getter(){
    return memory_size;
}
uint32_t page_size_getter(){
    return page_size;
}
uint32_t entries_per_table_getter(){
    return entries_per_table;
}
uint32_t memory_time(){
    return memory_delay_time;
}
char* algorithm(){
    return replacement_algorithm;
}
uint32_t swap_time(){
    return swap_delay_time;
}
uint32_t frames_per_table_getter(){
    return frames_per_process;
}
char* swap_path(){
    return swap_file_path;
}
uint32_t max_page_quantity(){
    return entries_per_table_getter() * entries_per_table_getter();
}
uint32_t quantity_memory_frames(){
        return memory_size_getter()/page_size_getter();
}
void free_memory_configuration_manager(){

    free_configuration_manager();
}
