#include "kernel_process_image.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "kernel_long_term_scheduler.h"
#include "kernel_configuration.h"
#include "kernel_sanitizer.h"

t_list* processes;
uint32_t process_counter;

void initialize_kernel_process_image() {
    processes = list_create();
    process_counter = 0;
}

void create_process_image(t_console_message *console_message) {
    t_process_image *process = safe_malloc(sizeof (t_process_image));
    t_pcb *pcb = process->pcb = safe_malloc(sizeof (t_pcb));
    pcb->pid = ++process_counter;
    pcb->process_size = console_message->process_size;
    pcb->instructions = console_message->instructions;
    pcb->pc = 0;
    pcb->next_burst = get_initial_estimation();
    list_add(processes, (void*) process);
    add_to_scheduler_queue(pcb, NEW);
    request_schedule_process();
}

t_process_image *find_process_image_by_pid(uint32_t pid) {

    bool _find_by_pid(t_process_image* process) {
        return process -> pcb -> pid == pid;
    }

    return list_find(processes, _find_by_pid);
}

void free_process_image_pcb(t_pcb* pcb) {
    bool _find_by_pid(t_process_image* process) {
        return process -> pcb -> pid == pcb->pid;
    }

    t_process_image * process_image = list_remove_by_condition(processes, _find_by_pid);
    free_pcb(process_image->pcb);
    free(process_image);
}

void free_process_image(t_process_image *process_image){
    free_pcb(process_image->pcb);
    free(process_image);
}

void free_kernel_process_image() {
    list_destroy_and_destroy_elements(processes, (void (*)(void *)) free_process_image);
}