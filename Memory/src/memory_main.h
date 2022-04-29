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
#include<commons/string.h>
#include<sys/mman.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>
//VARIABLES GLOBALES
void* BLOQUE_MEMORIA;
t_log* LOGGER;
t_config* CONFIG;
char* PATH_SWAP_BASE;
FILE* ARCHIVO;
void* BLOQUE_MEMORIA_SECUNDARIA;
//VARIABLES GLOBALES

//FUNCION INICIAL
t_log* initLogger(void);
t_config* initConfig(void);
void initGlobalVariables(void);
//FUNCION INICIAL

//INICIO, LECTURA Y ESCRITURA EN BLOQUE DE MEMORIA
void initMemory(void);
void saveValueInMemory(uint32_t* direccionFisica, uint32_t* valor,uint32_t* pid);
uint32_t* readValueInMemory(uint32_t* direccionFisica,uint32_t* pid);
//INICIO, LECTURA Y ESCRITURA EN BLOQUE DE MEMORIA

//MANEJO DE ARCHIVOS SWAP
void createFileSwap(uint32_t* pid);
char* generatePathFile(uint32_t* pid);
void saveContentInFile(uint32_t* direccionFisica,uint32_t* tamanio,uint32_t* pid);
void loadFileIntoMemory(uint32_t* pid,uint32_t* direccionFisica,uint32_t* tamanio);
//MANEJO DE ARCHIVOS SWAP
#endif //MEMORY_MEMORY_MAIN_H
