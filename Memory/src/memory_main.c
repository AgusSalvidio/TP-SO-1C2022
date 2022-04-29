#include "memory_main.h"

int main(void){
    initGlobalVariables();
    initMemory();
    /* TEST DE FUNCIONAMIENTO DE NUEVOS METODOS
    */
    saveValueInMemory(0,5,1);
    saveValueInMemory(4,6,1);
    saveValueInMemory(8,7,1);
    readValueInMemory(4,1);
    readValueInMemory(8,1);
    createFileSwap(1);
    saveContentInFile(0, 12,1);
    loadFileIntoMemory(1,12,12);
    readValueInMemory(12,1);
    readValueInMemory(16,1);
    readValueInMemory(20,1);

    return EXIT_SUCCESS;
}


void initMemory(){
    uint32_t* memoria = atoi(config_get_string_value(CONFIG,"TAM_MEMORIA"));
    BLOQUE_MEMORIA = malloc(memoria);
}

void initGlobalVariables(){
    LOGGER = initLogger();
    log_info(LOGGER,"Logger creado");
    CONFIG = initConfig();
    log_info(LOGGER,"Config creada");
    PATH_SWAP_BASE = config_get_string_value(CONFIG,"PATH_SWAP");
}

void saveValueInMemory(uint32_t* direccionFisica, uint32_t* valor,uint32_t* pid){
    memcpy(BLOQUE_MEMORIA+ (uint32_t)direccionFisica, &valor, sizeof(uint32_t));

    log_info(LOGGER,"Se guardo el valor %d en la direccion fisica %d para el PID %d",valor,direccionFisica,pid);
}

uint32_t* readValueInMemory(uint32_t* direccionFisica,uint32_t* pid){
    uint32_t* valor;
    memcpy(&valor,BLOQUE_MEMORIA +(uint32_t)direccionFisica,sizeof(uint32_t));
    log_info(LOGGER,"Se obtuvo el valor %d en la direccion fisica %d para el PID %d",valor,direccionFisica,pid);
    return valor;
}

void createFileSwap(uint32_t* pid){
    char* path = generatePathFile(pid);
    ARCHIVO = fopen(path,"w+");
    if(ARCHIVO == NULL){
        log_info(LOGGER,"No se pudo crear el archivo %s",path);
    }
    log_info(LOGGER,"Se creo el archivo %d.swap", pid);
    fclose(ARCHIVO);
    free(path);

}
char* generatePathFile(uint32_t* pid){
    char* path = string_new();
    string_append(&path, PATH_SWAP_BASE);
    string_append_with_format(&path,"/%d.swap",pid);
    return path;
}

void saveContentInFile(uint32_t* direccionFisica,uint32_t* tamanio,uint32_t* pid){
    //PRECONDICION: EL archivo para el pid ya debe existir
    char* path = generatePathFile(pid);
    ARCHIVO = open(path,O_CREAT|O_RDWR, 07777);
    if(ARCHIVO == -1){
        log_info(LOGGER,"No se pudo abrir el archivo %s",path);
    }else{
        //inicio de la memoria secundaria con mmap
        BLOQUE_MEMORIA_SECUNDARIA = mmap(NULL, tamanio,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_FILE,ARCHIVO, 0);
        ftruncate(ARCHIVO, tamanio);
        //memset(BLOQUE_MEMORIA_SECUNDARIA, ' ', TAMANIO_SWAP);
        //msync(BLOQUE_MEMORIA_SECUNDARIA, TAMANIO_MEMORIA_SECUNDARIA, MS_SYNC); //sincronizar
        memcpy(BLOQUE_MEMORIA_SECUNDARIA,BLOQUE_MEMORIA + (uint32_t)direccionFisica,tamanio);
        msync(BLOQUE_MEMORIA_SECUNDARIA, tamanio, MS_SYNC);
        close(ARCHIVO);

    }
}

void loadFileIntoMemory(uint32_t* pid,uint32_t* direccionFisica,uint32_t* tamanio){
    char* path = generatePathFile(pid);
    ARCHIVO = open(path,O_CREAT|O_RDWR, 07777);
    if(ARCHIVO == -1){
        log_info(LOGGER,"No se pudo abrir el archivo %s",path);
    }else{
        BLOQUE_MEMORIA_SECUNDARIA = mmap(NULL, (uint32_t)tamanio,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_FILE,ARCHIVO, 0);
        ftruncate(ARCHIVO, (uint32_t)tamanio);
        memcpy(BLOQUE_MEMORIA + (uint32_t)direccionFisica,BLOQUE_MEMORIA_SECUNDARIA,(uint32_t)tamanio);
        msync(BLOQUE_MEMORIA_SECUNDARIA, (uint32_t)tamanio, MS_SYNC);
        close(ARCHIVO);
    }
}

t_log* initLogger(){
    return log_create("../src/resources/memory.log", "Memory", true, LOG_LEVEL_INFO);
}


t_config* initConfig() //cambiar ruta a ubicacion del clone del repo
{
    return config_create("../src/resources/memory.config");
}
