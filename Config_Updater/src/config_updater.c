#include <string.h>
#include <commons/string.h>
#include <commons/config.h>
#include <stdlib.h>
#include<stdio.h>
#include <readline/readline.h>
#include "../../Utils/include/common_structures.h"

#define KERNEL_PATH "/home/utnso/tp-2022-2c-La-Scaloneta/Kernel/kernel.config"
#define MEMORY_PATH "/home/utnso/tp-2022-2c-La-Scaloneta/Memory/memory.config"
#define CPU_PATH "/home/utnso/tp-2022-2c-La-Scaloneta/CPU/cpu.config"

void free_system(){}

char ** get_parsed_command(char *command) {
    return string_n_split(command, 2, " ");
}

void handle_ip(){

    printf("Ingresar IP Memoria:\n");

    char* command = readline(">");
    char ** parsed_command = get_parsed_command(command);

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"IP_MEMORIA",parsed_command[0]);
    config_save(file_config);
    printf("Se guardó el valor de IP_MEMORIA en el archivo de configuracion de kernel.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"IP_MEMORIA",parsed_command[0]);
    config_save(file_config);
    printf("Se guardó el valor de IP_MEMORIA en el archivo de configuracion de CPU.\n");

    printf("Ingresar IP CPU:\n");

    parsed_command = get_parsed_command(command);
    file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"IP_CPU",parsed_command[0]);
    config_save(file_config);
    printf("Se guardó el valor de IP_CPU en el archivo de configuracion de kernel.\n");
    free(file_config);

    free_char_array(parsed_command);
    free(command);
    printf("Se volverà al menu principal.\n");
}

void handle_base_test(){

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"ALGORITMO_PLANIFICACION","FIFO");
    config_set_value(file_config,"ESTIMACION_INICIAL","10000");
    config_set_value(file_config,"ALFA","0.5");
    config_set_value(file_config,"GRADO_MULTIPROGRAMACION","4");
    config_set_value(file_config,"TIEMPO_MAXIMO_BLOQUEADO","100000");
    config_save(file_config);
    printf("Se guardó la configuracion de KERNEL.\n");

    file_config = config_create(MEMORY_PATH);
    config_set_value(file_config,"TAM_MEMORIA","4096");
    config_set_value(file_config,"TAM_PAGINA","64");
    config_set_value(file_config,"ENTRADAS_POR_TABLA","4");
    config_set_value(file_config,"RETARDO_MEMORIA","1000");
    config_set_value(file_config,"ALGORITMO_REEMPLAZO","CLOCK-M");
    config_set_value(file_config,"MARCOS_POR_PROCESO","4");
    config_set_value(file_config,"RETARDO_SWAP","2000");
    config_save(file_config);
    printf("Se guardó la configuracion de MEMORIA.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"ENTRADAS_TLB","4");
    config_set_value(file_config,"REEMPLAZO_TLB","LRU");
    config_set_value(file_config,"RETARDO_NOOP","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de CPU.\n");

    free(file_config);
    printf("Se volverà al menu principal.\n");

}

void handle_kernel_scheduler(char* algorithm){

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"ALGORITMO_PLANIFICACION",algorithm);
    config_set_value(file_config,"ESTIMACION_INICIAL","20000");
    config_set_value(file_config,"ALFA","0.5");
    config_set_value(file_config,"GRADO_MULTIPROGRAMACION","4");
    config_set_value(file_config,"TIEMPO_MAXIMO_BLOQUEADO","10000");
    config_save(file_config);
    printf("Se guardó la configuracion de KERNEL.\n");

    file_config = config_create(MEMORY_PATH);
    config_set_value(file_config,"TAM_MEMORIA","4096");
    config_set_value(file_config,"TAM_PAGINA","64");
    config_set_value(file_config,"ENTRADAS_POR_TABLA","4");
    config_set_value(file_config,"RETARDO_MEMORIA","1000");
    config_set_value(file_config,"ALGORITMO_REEMPLAZO","CLOCK-M");
    config_set_value(file_config,"MARCOS_POR_PROCESO","4");
    config_set_value(file_config,"RETARDO_SWAP","2000");
    config_save(file_config);
    printf("Se guardó la configuracion de MEMORIA.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"ENTRADAS_TLB","4");
    config_set_value(file_config,"REEMPLAZO_TLB","LRU");
    config_set_value(file_config,"RETARDO_NOOP","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de CPU.\n");

    free(file_config);
    printf("Se volverà al menu principal.\n");



}

void handle_kernel_suspend(char* algorithm){

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"ALGORITMO_PLANIFICACION",algorithm);
    config_set_value(file_config,"ESTIMACION_INICIAL","20000");
    config_set_value(file_config,"ALFA","0.5");
    config_set_value(file_config,"GRADO_MULTIPROGRAMACION","2");
    config_set_value(file_config,"TIEMPO_MAXIMO_BLOQUEADO","8000");
    config_save(file_config);
    printf("Se guardó la configuracion de KERNEL.\n");

    file_config = config_create(MEMORY_PATH);
    config_set_value(file_config,"TAM_MEMORIA","4096");
    config_set_value(file_config,"TAM_PAGINA","64");
    config_set_value(file_config,"ENTRADAS_POR_TABLA","4");
    config_set_value(file_config,"RETARDO_MEMORIA","1000");
    config_set_value(file_config,"ALGORITMO_REEMPLAZO","CLOCK-M");
    config_set_value(file_config,"MARCOS_POR_PROCESO","4");
    config_set_value(file_config,"RETARDO_SWAP","2000");
    config_save(file_config);
    printf("Se guardó la configuracion de MEMORIA.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"ENTRADAS_TLB","4");
    config_set_value(file_config,"REEMPLAZO_TLB","LRU");
    config_set_value(file_config,"RETARDO_NOOP","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de CPU.\n");

    free(file_config);
    printf("Se volverà al menu principal.\n");


}

void handle_memory(char* algorithm){

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"ALGORITMO_PLANIFICACION","FIFO");
    config_set_value(file_config,"ESTIMACION_INICIAL","20000");
    config_set_value(file_config,"ALFA","0.5");
    config_set_value(file_config,"GRADO_MULTIPROGRAMACION","2");
    config_set_value(file_config,"TIEMPO_MAXIMO_BLOQUEADO","5000");
    config_save(file_config);
    printf("Se guardó la configuracion de KERNEL.\n");

    file_config = config_create(MEMORY_PATH);
    config_set_value(file_config,"TAM_MEMORIA","2048");
    config_set_value(file_config,"TAM_PAGINA","256");
    config_set_value(file_config,"ENTRADAS_POR_TABLA","4");
    config_set_value(file_config,"RETARDO_MEMORIA","1000");
    config_set_value(file_config,"ALGORITMO_REEMPLAZO",algorithm);
    config_set_value(file_config,"MARCOS_POR_PROCESO","3");
    config_set_value(file_config,"RETARDO_SWAP","5000");
    config_save(file_config);
    printf("Se guardó la configuracion de MEMORIA.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"ENTRADAS_TLB","1");
    config_set_value(file_config,"REEMPLAZO_TLB","LRU");
    config_set_value(file_config,"RETARDO_NOOP","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de CPU.\n");

    free(file_config);
    printf("Se volverà al menu principal.\n");


}

void handle_tlb(char* algorithm){

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"ALGORITMO_PLANIFICACION","FIFO");
    config_set_value(file_config,"ESTIMACION_INICIAL","20000");
    config_set_value(file_config,"ALFA","0.5");
    config_set_value(file_config,"GRADO_MULTIPROGRAMACION","2");
    config_set_value(file_config,"TIEMPO_MAXIMO_BLOQUEADO","5000");
    config_save(file_config);
    printf("Se guardó la configuracion de KERNEL.\n");

    file_config = config_create(MEMORY_PATH);
    config_set_value(file_config,"TAM_MEMORIA","8192");
    config_set_value(file_config,"TAM_PAGINA","128");
    config_set_value(file_config,"ENTRADAS_POR_TABLA","4");
    config_set_value(file_config,"RETARDO_MEMORIA","3000");
    config_set_value(file_config,"ALGORITMO_REEMPLAZO","CLOCK");
    config_set_value(file_config,"MARCOS_POR_PROCESO","16");
    config_set_value(file_config,"RETARDO_SWAP","3000");
    config_save(file_config);
    printf("Se guardó la configuracion de MEMORIA.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"ENTRADAS_TLB","4");
    config_set_value(file_config,"REEMPLAZO_TLB",algorithm);
    config_set_value(file_config,"RETARDO_NOOP","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de CPU.\n");

    free(file_config);
    printf("Se volverà al menu principal.\n");

}
void handle_last_test(){

    t_config* file_config = config_create(KERNEL_PATH);
    config_set_value(file_config,"ALGORITMO_PLANIFICACION","SRT");
    config_set_value(file_config,"ESTIMACION_INICIAL","50000");
    config_set_value(file_config,"ALFA","0.2");
    config_set_value(file_config,"GRADO_MULTIPROGRAMACION","6");
    config_set_value(file_config,"TIEMPO_MAXIMO_BLOQUEADO","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de KERNEL.\n");

    file_config = config_create(MEMORY_PATH);
    config_set_value(file_config,"TAM_MEMORIA","10240");
    config_set_value(file_config,"TAM_PAGINA","256");
    config_set_value(file_config,"ENTRADAS_POR_TABLA","4");
    config_set_value(file_config,"RETARDO_MEMORIA","500");
    config_set_value(file_config,"ALGORITMO_REEMPLAZO","CLOCK-M");
    config_set_value(file_config,"MARCOS_POR_PROCESO","4");
    config_set_value(file_config,"RETARDO_SWAP","1000");
    config_save(file_config);
    printf("Se guardó la configuracion de MEMORIA.\n");

    file_config = config_create(CPU_PATH);
    config_set_value(file_config,"ENTRADAS_TLB","2");
    config_set_value(file_config,"REEMPLAZO_TLB","LRU");
    config_set_value(file_config,"RETARDO_NOOP","100");
    config_save(file_config);
    printf("Se guardó la configuracion de CPU.\n");

    free(file_config);
    printf("Se volverà al menu principal.\n");


}

void handle_config(){

    printf("0. Prueba Base\n");
    printf("1. Prueba Kernel - Planificacion FIFO\n");
    printf("2. Prueba Kernel - Planificacion SRT\n");
    printf("3. Prueba Kernel - Suspension FIFO\n");
    printf("4. Prueba Kernel - Suspension SRT\n");
    printf("5. Prueba Memoria - CLOCK\n");
    printf("6. Prueba Memoria - CLOCK-M\n");
    printf("7. Prueba TLB - FIFO\n");
    printf("8. Prueba TLB - LRU\n");
    printf("9. Prueba FINAL\n");

    char* command = readline(">");
    char ** parsed_command = get_parsed_command(command);

    if (strcmp("0", parsed_command[0]) == 0) {
        handle_base_test();
    }else if (strcmp("1", parsed_command[0]) == 0) {
        handle_kernel_scheduler("FIFO");
    }else if (strcmp("2", parsed_command[0]) == 0) {
        handle_kernel_scheduler("SRT");
    }else if (strcmp("3", parsed_command[0]) == 0) {
        handle_kernel_suspend("FIFO");
    }else if (strcmp("4", parsed_command[0]) == 0) {
        handle_kernel_suspend("SRT");
    }else if (strcmp("5", parsed_command[0]) == 0) {
        handle_memory("CLOCK");
    }else if (strcmp("6", parsed_command[0]) == 0) {
        handle_memory("CLOCK-M");
    }else if (strcmp("7", parsed_command[0]) == 0) {
        handle_tlb("FIFO");
    }else if (strcmp("8", parsed_command[0]) == 0) {
        handle_tlb("LRU");
    }else if (strcmp("9", parsed_command[0]) == 0) {
        handle_last_test();
    }

    free_char_array(parsed_command);
    free(command);
    printf("Se volverà al menu principal.\n");
}

void initialize_updater_console(){

    printf("1. Modificar IPs modulos\n");
    printf("2. Modificar Config pruebas\n");
    printf("3. Salir\n");

    char* command = readline(">");
    char ** parsed_command = get_parsed_command(command);
    while (strcmp("3", parsed_command[0]) != 0) {

        if (strcmp("1", parsed_command[0]) == 0) {
            handle_ip();
        }else if (strcmp("2", parsed_command[0]) == 0) {
            handle_config();
        }
        free_char_array(parsed_command);
        free(command);

        printf("1. Modificar IPs modulos.\n");
        printf("2. Modificar Config pruebas .\n");
        printf("3. Salir .\n");

        command = readline(">");
        parsed_command = get_parsed_command(command);
    }
    free_char_array(parsed_command);
    free(command);
}

int main (void) {

    initialize_updater_console();

    return EXIT_SUCCESS;
}



