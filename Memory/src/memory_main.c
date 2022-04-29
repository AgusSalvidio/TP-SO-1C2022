#include "memory_main.h"

int main(void){
    iniciarVariablesGlobales();
    reservarMemoria();
    /* TEST DE FUNCIONAMIENTO DE NUEVOS METODOS
    guardarValorEnMemoria(0,5,1);
    guardarValorEnMemoria(4,6,1);
    guardarValorEnMemoria(4,6,1);
    guardarValorEnMemoria(8,7,1);
    uint32_t valor = leerValorEnMemoria(0,1);
    log_info(LOGGER, "VALOR= %d",valor);
    valor = leerValorEnMemoria(4,1);
    log_info(LOGGER, "VALOR= %d",valor);
    valor = leerValorEnMemoria(8,1);
    log_info(LOGGER, "VALOR= %d",valor);
    crearArchivoSwap(1);
    guardarContenidoEnArchivo(0, 12,1);
    cargarArchivoEnMemoria(1,12,12);
    valor = leerValorEnMemoria(12,1);
    log_info(LOGGER, "VALOR= %d",valor);
    valor = leerValorEnMemoria(16,1);
    log_info(LOGGER, "VALOR= %d",valor);
    valor = leerValorEnMemoria(20,1);
    log_info(LOGGER, "VALOR= %d",valor);
     */
    return EXIT_SUCCESS;
}


void reservarMemoria(){
    uint32_t memoria = atoi(config_get_string_value(CONFIG,"TAM_MEMORIA"));
    BLOQUE_MEMORIA = malloc(memoria);
}

void iniciarVariablesGlobales(){
    LOGGER = iniciarLogger();
    log_info(LOGGER,"Logger creado");
    CONFIG = iniciarConfig();
    log_info(LOGGER,"Config creada");
    PATH_SWAP_BASE = config_get_string_value(CONFIG,"PATH_SWAP");
}

void guardarValorEnMemoria(uint32_t direccionFisica, uint32_t valor,uint32_t pid){
    memcpy(BLOQUE_MEMORIA+direccionFisica, &valor, sizeof(uint32_t));

    log_info(LOGGER,"Se guardo el valor %d en la direccion fisica %d para el PID %d",valor,direccionFisica,pid);
}

uint32_t leerValorEnMemoria(uint32_t direccionFisica,uint32_t pid){
    uint32_t valor;
    memcpy(&valor,BLOQUE_MEMORIA +direccionFisica,sizeof(uint32_t));
    log_info(LOGGER,"Se obtuvo el valor %d en la direccion fisica %d para el PID %d",valor,direccionFisica,pid);
    return valor;
}

void crearArchivoSwap(uint32_t pid){
    char* path = generarPathArchivo(pid);
    ARCHIVO = fopen(path,"w+");
    if(ARCHIVO == NULL){
        log_info(LOGGER,"No se pudo crear el archivo %s",path);
    }
    log_info(LOGGER,"Se creo el archivo %d.swap", pid);
    fclose(ARCHIVO);
    free(path);

}
char* generarPathArchivo(uint32_t pid){
    char* path = string_new();
    string_append(&path, PATH_SWAP_BASE);
    string_append_with_format(&path,"/%d.swap",pid);
    return path;
}

void guardarContenidoEnArchivo(uint32_t direccionFisica,uint32_t tamanio,uint32_t pid){
    //PRECONDICION: EL archivo para el pid ya debe existir
    char* path = generarPathArchivo(pid);
    ARCHIVO = open(path,O_CREAT|O_RDWR, 07777);
    if(ARCHIVO == -1){
        log_info(LOGGER,"No se pudo abrir el archivo %s",path);
    }else{
        //inicio de la memoria secundaria con mmap
        BLOQUE_MEMORIA_SECUNDARIA = mmap(NULL, tamanio,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_FILE,ARCHIVO, 0);
        ftruncate(ARCHIVO, tamanio);
        //memset(BLOQUE_MEMORIA_SECUNDARIA, ' ', TAMANIO_SWAP);
        //msync(BLOQUE_MEMORIA_SECUNDARIA, TAMANIO_MEMORIA_SECUNDARIA, MS_SYNC); //sincronizar
        memcpy(BLOQUE_MEMORIA_SECUNDARIA,BLOQUE_MEMORIA + direccionFisica,tamanio);
        msync(BLOQUE_MEMORIA_SECUNDARIA, tamanio, MS_SYNC);
        close(ARCHIVO);

    }
}

void cargarArchivoEnMemoria(uint32_t pid,uint32_t direccionFisica,uint32_t tamanio){
    char* path = generarPathArchivo(pid);
    ARCHIVO = open(path,O_CREAT|O_RDWR, 07777);
    if(ARCHIVO == -1){
        log_info(LOGGER,"No se pudo abrir el archivo %s",path);
    }else{
        BLOQUE_MEMORIA_SECUNDARIA = mmap(NULL, tamanio,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_FILE,ARCHIVO, 0);
        ftruncate(ARCHIVO, tamanio);
        memcpy(BLOQUE_MEMORIA + direccionFisica,BLOQUE_MEMORIA_SECUNDARIA,tamanio);
        msync(BLOQUE_MEMORIA_SECUNDARIA, tamanio, MS_SYNC);
        close(ARCHIVO);
    }
}

t_log* iniciarLogger(){
    return log_create("../src/resources/memory.log", "Memory", true, LOG_LEVEL_INFO);
}


t_config* iniciarConfig() //cambiar ruta a ubicacion del clone del repo
{
    return config_create("../src/resources/memory.config");
}
