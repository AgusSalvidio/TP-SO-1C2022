#include "kernel_process_image.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"

t_list* processes;
uint32_t process_counter;

void initialize_kernel_process_image() {
    processes = list_create();
    process_counter = 0;
}

void create_process_image(t_console_message *console_message) {
    t_process_image *process = safe_malloc(sizeof (t_process_image));
    process->pcb->pid = ++process_counter;
    process->state = NEW;
    list_add(processes, (void*) process);
}

t_process_image *find_process_image_by_pid(uint32_t pid) {

    bool _find_by_pid(t_process_image* process) {
        return process -> pcb -> pid == pid;
    }

    return list_find(processes, _find_by_pid);
}

void free_process_image(t_process_image *process_image){
    free(process_image);
}

void free_kernel_process_image() {
    list_destroy_and_destroy_elements(processes, (void (*)(void *)) free_process_image);
}