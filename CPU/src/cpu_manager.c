#include <cpu_manager.h>
#include "cpu_logs_manager.h"
#include "cpu_configuration_manager.h"
#include "cpu_memory_connection_handler.h"
#include "cpu_kernel_interrupt_connection_handler.h"
#include "cpu_instruction_cycle.h"
#include "cpu_mmu.h"
#include "../../Utils/include/logger.h"
#include "cpu_tlb.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <unistd.h>
#include <commons/log.h>


void wait_delay_time(){
    uint32_t delay_time_in_seconds = get_noop_delay()/1000;
    sleep(delay_time_in_seconds);
}

t_request* request_to_send_using(void* received_structure, uint32_t operation){
    t_request* request_to_send = safe_malloc(sizeof(t_request));
    request_to_send -> operation = operation;
    request_to_send -> structure = received_structure;
    request_to_send -> sanitizer_function = free;
    return request_to_send;
}

void* handle_read_request_procedure(uint32_t table_index, t_list* operands){
    log_current_instruction_running("Read");
    uint32_t logical_address = (uint32_t) list_get(operands, 0);
    t_physical_address* physical_address = address_translator_management(table_index, logical_address);

    consider_as_garbage(physical_address, free);
    log_read_content(send_read_to_memory(physical_address));
}

void* handle_write_request_procedure(uint32_t table_index, t_list* operands){
    log_current_instruction_running("Write");
    uint32_t logical_address = (uint32_t) list_get(operands, 0);
    uint32_t value = (uint32_t) list_get(operands, 1);
    t_physical_address* physical_address = address_translator_management(table_index, logical_address);

    consider_as_garbage(physical_address, free);
    send_write_to_memory(physical_address, value);
}

void* handle_copy_request_procedure(uint32_t table_index, t_list* operands){
    log_current_instruction_running("Copy");
    uint32_t logical_address = (uint32_t) list_get(operands, 0);
    uint32_t value = (uint32_t) list_get(operands, 1);
    t_physical_address* physical_address = address_translator_management(table_index, logical_address);

    consider_as_garbage(physical_address, free);
    send_copy_to_memory(physical_address, value);
}


void* handle_no_op_request_procedure(uint32_t pid, t_list* operands){
    log_current_instruction_running("No op");
    wait_delay_time();
}

void* request_reponse_of_instruction_for_pcb(t_instruction* instruction, t_pcb* current_pcb){
    if(instruction -> type == IO){
        log_current_instruction_running("I/O");
        t_io_pcb* io_pcb = safe_malloc(sizeof(t_io_pcb));
        io_pcb -> pcb = current_pcb;
        io_pcb -> blocked_time = (uint32_t)list_get(instruction->operands, 0);

        list_clean(tlb());

        log_return_pcb_to_kernel();
        consider_as_garbage(io_pcb, free);
        return request_to_send_using(io_pcb, IO_PCB);
    }
    if(instruction -> type == EXIT) {
        log_current_instruction_running("Exit");
        list_clean(tlb());

        log_return_pcb_to_kernel();
        return request_to_send_using(current_pcb, PCB);
    }
    else
        return NULL;
}

void* handle_PCB_request_procedure(t_pcb* current_pcb){
    while(!current_interruption_status()){
        uint32_t table_index = current_pcb -> page_table;
        t_instruction* instruction = fetch_instruction(current_pcb);
        decode_instruction(table_index, instruction);

        t_request * request_response = request_reponse_of_instruction_for_pcb(instruction, current_pcb);
        log_debug(process_execution_logger(), "instruction: %d",instruction->type);
        if(request_response != NULL)
            return request_response;
        else
            execute(table_index, instruction);
    }
    modify_interruption_status();
    return request_to_send_using(current_pcb, PCB);
}

void free_cpu_manager(){};