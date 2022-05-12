#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <stddef.h>
#include <semaphore.h>
#include "serialization_interface.h"

#define ever (;;)


typedef struct Handshake{
    uint32_t page_size;
    uint32_t entries_per_page;
}t_handshake;

typedef struct Read{
    uint32_t logical_address;
}t_read;

typedef struct Write{
    uint32_t logical_address;
    uint32_t value;
}t_write;

typedef struct Copy{
    uint32_t destiny_logical_address;
    uint32_t origin_logical_address;
}t_copy;

enum Operation {
    HANDSHAKE, READ, WRITE, COPY, NO_OP, IO, EXIT
};

void initialize_signal_handler();
void handle_signal(int signal_number, void (*handler_function) ());

void safe_sem_initialize(sem_t* semaphore);
void safe_sem_initialize_with_value(sem_t* semaphore, uint32_t value);
void safe_sem_wait(sem_t* semaphore);
void safe_sem_post(sem_t* semaphore);
void safe_sem_destroy(sem_t* semaphore);

void* safe_malloc(size_t size);

char* process_description_for(char* process_name, t_list* strings_to_hash);
uint64_t current_time_in_milliseconds();
void assert_only_one_in(t_list* self);

void free_system_debugging_thread_alive_for(int seconds);

void sleep_for(int reconnection_time_in_seconds);
int minimum_integer_between(int number, int another_number);

void free_char_array(char**);

#endif //COMMON_STRUCTURES_H
