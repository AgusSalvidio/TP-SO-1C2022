#include "memory_main.h"

int main(void){
    iniciarVariablesGlobales();
    reservarMemoria();
    guardarValorEnMemoria((uint32_t )0,(uint32_t )5,(uint32_t )1);
    uint32_t valor = leerValorEnMemoria(0,1);
    log_info(LOGGER, "VALOR= %d",valor);
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
}

void guardarValorEnMemoria(uint32_t direccionFisica, uint32_t valor,uint32_t pid){
    memcpy(BLOQUE_MEMORIA+direccionFisica, &valor, sizeof(uint32_t));

    log_info(LOGGER,"Se guardo el valor %d en la direccion fisica %d para el PID %d",valor,direccionFisica,pid);
}

uint32_t leerValorEnMemoria(uint32_t direccionFisica,uint32_t pid){
    uint32_t valor;
    memcpy(&valor,BLOQUE_MEMORIA ,sizeof(uint32_t));
    log_info(LOGGER,"Se obtuvo el valor %d en la direccion fisica %d para el PID %d",valor,direccionFisica,pid);
    return valor;
}


t_log* iniciarLogger(){
    return log_create("../src/resources/memory.log", "Memory", true, LOG_LEVEL_INFO);
}


t_config* iniciarConfig() //cambiar ruta a ubicacion del clone del repo
{
    return config_create("../src/resources/memory.config");
}
