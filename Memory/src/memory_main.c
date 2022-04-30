#include "memory_main.h"
int main(void){
    init_global_variables();
    init_memory();
    /* TEST DE FUNCIONAMIENTO DE NUEVOS METODOS
    */
    save_value_in_memory(0,5,1);
    save_value_in_memory(4,6,1);
    save_value_in_memory(8,7,1);
    read_value_in_memory(4,1);
    read_value_in_memory(8,1);
    create_swap_file(1);
    save_content_in_swap_file(0, 12,1);
    load_file_into_memory(1,12,12);
    read_value_in_memory(12,1);
    read_value_in_memory(16,1);
    read_value_in_memory(20,1);

    return EXIT_SUCCESS;
}


void init_memory(){
    uint32_t size = atoi(config_get_string_value(CONFIG,"TAM_MEMORIA"));
    MEMORY_BLOCK = malloc(size);
}

void init_global_variables(){
    LOGGER = init_logger();
    log_info(LOGGER,"Logger created");
    CONFIG = init_config();
    log_info(LOGGER,"Config created");
    PATH_SWAP_BASE = config_get_string_value(CONFIG,"PATH_SWAP");
}

void save_value_in_memory(uint32_t direction, uint32_t value,uint32_t pid){
    memcpy(MEMORY_BLOCK+ direction, &value, sizeof(uint32_t));

    log_info(LOGGER,"The value %d was saved at direction %d for PID %d",value,direction,pid);
}

uint32_t read_value_in_memory(uint32_t direction,uint32_t pid){
    uint32_t value;
    memcpy(&value,MEMORY_BLOCK +direction,sizeof(uint32_t));
    log_info(LOGGER,"The value %d obtained in direction %d for PID %d",value,direction,pid);
    return value;
}

t_log* init_logger(){
    return log_create("../src/resources/memory.log", "Memory", true, LOG_LEVEL_INFO);
}


t_config* init_config() //cambiar ruta a ubicacion del clone del repo
{
    return config_create("../src/resources/memory.config");
}

void create_swap_file(uint32_t pid){
    char* path = generate_path_file(pid);
    SWAP_FILE = fopen(path,"w+");
    if(SWAP_FILE == NULL){
        log_info(LOGGER,"could not create  SWAP_FILE %s",path);
    }
    log_info(LOGGER,"SWAP_FILE created: %d.swap", pid);
    fclose(SWAP_FILE);
    free(path);

}
char* generate_path_file(uint32_t pid){
    char* path = string_new();
    string_append(&path, PATH_SWAP_BASE);
    string_append_with_format(&path,"/%d.swap",pid);
    return path;
}

void save_content_in_swap_file(uint32_t direction,uint32_t size,uint32_t pid){
    //PRECONDICION: EL SWAP_FILE para el pid ya debe existir
    char* path = generate_path_file(pid);
    SWAP_FILE = open(path,O_CREAT|O_RDWR, 07777);
    if(SWAP_FILE == -1){
        log_info(LOGGER,"No se pudo abrir el SWAP_FILE %s",path);
    }else{
        //inicio de la memoria SECONDARY con mmap
        MEMORY_BLOCK_SECONDARY = mmap(NULL, size,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_FILE,SWAP_FILE, 0);
        ftruncate(SWAP_FILE, size);
        //memset(MEMORY_BLOCK_SECONDARY, ' ', size_SWAP);
        //msync(MEMORY_BLOCK_SECONDARY, size_MEMORIA_SECONDARY, MS_SYNC); //sincronizar
        memcpy(MEMORY_BLOCK_SECONDARY,MEMORY_BLOCK + direction,size);
        msync(MEMORY_BLOCK_SECONDARY, size, MS_SYNC);
        close(SWAP_FILE);

    }
}

void load_file_into_memory(uint32_t pid,uint32_t direction,uint32_t size){
    char* path = generate_path_file(pid);
    SWAP_FILE = open(path,O_CREAT|O_RDWR, 07777);
    if(SWAP_FILE == -1){
        log_info(LOGGER,"Could not open SWAP_FILE %s",path);
    }else{
        MEMORY_BLOCK_SECONDARY = mmap(NULL, size,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_FILE,SWAP_FILE, 0);
        ftruncate(SWAP_FILE, size);
        memcpy(MEMORY_BLOCK + direction,MEMORY_BLOCK_SECONDARY,size);
        msync(MEMORY_BLOCK_SECONDARY, size, MS_SYNC);
        close(SWAP_FILE);
    }
}
