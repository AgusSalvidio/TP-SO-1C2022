#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <stddef.h>
#include <semaphore.h>
#include "serialization_interface.h"

#define ever (;;)

typedef struct Handshake {
    uint32_t page_size;
    uint32_t entries_per_page;
} t_handshake;

typedef struct Request_Response {
    char *type_description;
    char *content;
} t_request_response;

typedef struct Physical_address{
    uint32_t frame;
    uint32_t offset;
}t_physical_address;

typedef struct Read{
    t_physical_address* physical_address;
}t_read;

typedef struct Write{
    t_physical_address* physical_address;
    uint32_t value;
}t_write, t_copy;

typedef struct Level_paging{
    uint32_t first_level_entry;
    uint32_t second_level_entry;
    uint32_t offset;
}t_level_paging;

typedef struct MMU_access{
    uint32_t index;
    uint32_t entry;
}t_mmu_access;

typedef struct Process_Initialize {
    uint32_t pid;
    uint32_t process_size;
} t_initialize_process;

typedef struct Process_Suspend {
    uint32_t pid;
} t_suspend_process, t_finalize_process;

enum Operation {
    CONSOLE_MESSAGE, INSTRUCTION, HANDSHAKE,
    REQUEST_RESPONSE, READ, WRITE,
    COPY, NO_OP, IO,
    EXIT, INITIALIZE_PROCESS, SUSPEND_PROCESS,
    FINALIZE_PROCESS, INTERRUPTION, PCB, IO_PCB, FIRST_ACCESS, SECOND_ACCESS
};

typedef struct Console_message {
    uint32_t process_size;
    t_list *instructions;
} t_console_message;

typedef struct Instruction {
    uint32_t type;
    t_list *operands;
} t_instruction;

typedef struct Pcb{
    uint32_t pid;
    uint32_t process_size;
    t_list* instructions;
    uint32_t pc;
    uint32_t page_table;
    double next_burst;
} t_pcb;

typedef struct IO_pcb{
    t_pcb* pcb;
    uint32_t blocked_time;
}t_io_pcb;

void initialize_signal_handler();
void handle_signal(int signal_number, void (*handler_function) ());

void safe_sem_initialize(sem_t* semaphore);
void safe_sem_initialize_with_value(sem_t* semaphore, uint32_t value);
void safe_sem_wait(sem_t* semaphore);
void safe_sem_post(sem_t* semaphore);
void safe_sem_destroy(sem_t* semaphore);

void* safe_malloc(size_t size);

char* process_description_for(char* process_name, t_list* strings_to_hash);
uint32_t current_time_in_milliseconds();
void assert_only_one_in(t_list* self);

void free_system_debugging_thread_alive_for(int seconds);

void sleep_for(int reconnection_time_in_seconds);
int minimum_integer_between(int number, int another_number);

void free_char_array(char**);

#endif //COMMON_STRUCTURES_H
