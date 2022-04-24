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
void* BLOQUE_MEMORIA;
t_log* LOGGER;
t_config* CONFIG;


t_log* iniciarLogger(void);
t_config* iniciarConfig(void);
void iniciarVariablesGlobales(void);
void reservarMemoria(void);
void guardarValorEnMemoria(uint32_t direccionFisica, uint32_t valor,uint32_t pid);
uint32_t leerValorEnMemoria(uint32_t direccionFisica,uint32_t pid);
#endif //MEMORY_MEMORY_MAIN_H
