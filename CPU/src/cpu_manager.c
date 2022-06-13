#include <cpu_manager.h>
#include "cpu_logs_manager.h"
#include "cpu_configuration_manager.h"
#include "cpu_memory_connection_handler.h"
#include "cpu_kernel_interrupt_connection_handler.h"
#include "cpu_instruction_cycle.h"
#include <stdlib.h>
#include <unistd.h>

void wait_delay_time(){
    uint32_t delay_time_in_seconds = get_noop_delay()/1000;
    sleep(delay_time_in_seconds);
}

t_request* response_request_with(void* received_structure, uint32_t operation){
    t_request* request_to_send = safe_malloc(sizeof(t_request));
    request_to_send -> operation = operation;
    request_to_send -> structure = received_structure;
    request_to_send -> sanitizer_function = free;
    return request_to_send;
}

void* handle_read_request_procedure(uint32_t pid, t_list* operands){
    uint32_t logical_address = (uint32_t) list_get(operands, 0);
    send_read_to_memory(pid, logical_address);
    log_read_content(receive_read_content_from_memory());
}

void* handle_write_request_procedure(uint32_t pid, t_list* operands){
    uint32_t logical_address = (uint32_t) list_get(operands, 0);
    uint32_t value = (uint32_t) list_get(operands, 1);
    send_write_to_memory(pid, logical_address, value);
}

void* handle_copy_request_procedure(uint32_t pid, t_list* operands){
    uint32_t logical_address = (uint32_t) list_get(operands, 0);
    uint32_t value = (uint32_t) list_get(operands, 1);
    send_copy_to_memory(pid, logical_address, value);
}

void* handle_no_op_request_procedure(uint32_t pid, t_list* operands){
    wait_delay_time();
}

void* request_reponse_of_instruction_for_pcb(t_instruction* instruction, t_pcb* current_pcb){
    t_request_response* request_response = safe_malloc(sizeof(t_request_response));
    if(instruction -> type == IO){
        t_io_pcb* io_pcb = safe_malloc(sizeof(t_io_pcb));
        io_pcb -> pcb = current_pcb;
        io_pcb -> blocked_time = (uint32_t)list_get(instruction->operands, 0);

        request_response = response_request_with(io_pcb, IO_PCB);
    }
    if(instruction -> type == EXIT)
        request_response = response_request_with(current_pcb, PCB);
    else
        return NULL;

    return request_response;
}

void* handle_PCB_request_procedure(t_pcb* current_pcb){
    while(!current_interruption_status()){
        uint32_t pid = current_pcb -> pid;
        t_instruction* instruction = fetch_instruction(current_pcb);
        decode_instruction(pid, instruction);

        t_request_response* request_response = safe_malloc(sizeof(t_request_response));
        request_response = request_reponse_of_instruction_for_pcb(instruction, current_pcb);
        if(request_response != NULL)
            return request_response;
        else
            execute(pid, instruction);
    }
    modify_interruption_status();
    return response_request_with(current_pcb, PCB);
}

void free_cpu_manager(){};