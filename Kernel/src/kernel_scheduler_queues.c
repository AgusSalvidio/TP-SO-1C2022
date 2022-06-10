#include "kernel_scheduler_queues.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "kernel_sanitizer.h"
#include "kernel_logs_manager.h"
#include "kernel_process_image.h"

t_list* scheduler_queues;

t_scheduler_queue * new_scheduler_queue_for(uint32_t state){

    pthread_mutex_t mutex;
    safe_mutex_initialize(&mutex);
    t_scheduler_queue* scheduler_queue = safe_malloc(sizeof(t_scheduler_queue));
    scheduler_queue -> state = state;
    scheduler_queue -> mutex = mutex;
    scheduler_queue -> pcb_list = list_create();

    return scheduler_queue;
}

void initialize_scheduler_queues() {

    scheduler_queues = list_create();

    list_add(scheduler_queues, new_scheduler_queue_for(NEW));
    list_add(scheduler_queues, new_scheduler_queue_for(READY));
    list_add(scheduler_queues, new_scheduler_queue_for(EXEC));
    list_add(scheduler_queues, new_scheduler_queue_for(BLOCKED));
    list_add(scheduler_queues, new_scheduler_queue_for(SUSPENDED_BLOCKED));
    list_add(scheduler_queues, new_scheduler_queue_for(SUSPENDED_READY));
    list_add(scheduler_queues, new_scheduler_queue_for(Q_EXIT));

}

t_scheduler_queue *scheduler_queue_of(uint32_t state) {
    bool _is_for(t_scheduler_queue* scheduler_queue){
        return scheduler_queue -> state == state;
    }

    t_scheduler_queue* scheduler_queue_found =
            list_find(scheduler_queues, (bool (*)(void *)) _is_for);

    if(!scheduler_queue_found){
        log_invalid_state_error();
        free_system();
    }

    return scheduler_queue_found;
}

t_list *pcbs_in(uint32_t state) {
    return scheduler_queue_of(state) -> pcb_list;
}

uint32_t amount_of_pcbs_in(uint32_t state) {
    return list_size(pcbs_in(state));
}

void handling_concurrency_do(t_scheduler_queue *queue, void (*function)()) {
    safe_mutex_lock(&queue -> mutex);
    function();
    safe_mutex_unlock(&queue -> mutex);
}


char *name_of_state(uint32_t to_state) {
    switch (to_state) {
        case NEW:
            return "NEW";
        case READY:
            return "READY";
        case EXEC:
            return "EXEC";
        case BLOCKED:
            return "BLOCKED";
        case SUSPENDED_BLOCKED:
            return "SUSPENDED_BLOCKED";
        case SUSPENDED_READY:
            return "SUSPENDED_READY";
        case Q_EXIT:
            return "EXIT";
    }
}

void move_to_execute() {
    t_scheduler_queue * scheduler_queue = scheduler_queue_of(READY);
    t_pcb *pcb;

    void _remove(){
        pcb = list_remove_first(scheduler_queue -> pcb_list);
    }

    handling_concurrency_do(scheduler_queue, _remove);
    add_to_scheduler_queue(pcb, EXEC);
}

void move_to(t_pcb *pcb, uint32_t to_state) {
    remove_from_scheduler_queue(pcb);
    add_to_scheduler_queue(pcb, to_state);
}

void add_to_scheduler_queue(t_pcb* pcb, uint32_t to_state) {
    t_scheduler_queue * queue = scheduler_queue_of(to_state);

    void _add(){
        list_add(queue -> pcb_list, pcb);
    }

    handling_concurrency_do(queue, _add);
    t_process_image *image = find_process_image_by_pid(pcb->pid);
    image -> state = to_state;
}

void remove_from(t_list* pcb_list, t_pcb * pcb){

    bool _are_equals(t_pcb* pcb_to_compare){
        return pcb -> pid == pcb_to_compare -> pid;
    }

    t_pcb* pcb_found =
            list_remove_by_condition(pcb_list, (bool (*)(void *)) _are_equals);

    if (!pcb_found) {
        log_pcb_not_found_error(pcb -> pid);
        free_system();
    }
}

void remove_from_scheduler_queue(t_pcb * pcb) {
    t_process_image *image = find_process_image_by_pid(pcb->pid);
    t_scheduler_queue * queue = scheduler_queue_of(image -> state);

    void _remove(){
        remove_from(queue -> pcb_list, pcb);
    }

    handling_concurrency_do(queue, _remove);
}

void free_dispatcher_queue(t_scheduler_queue * scheduler_queue){
    safe_mutex_destroy(&scheduler_queue -> mutex);
    list_destroy(scheduler_queue -> pcb_list);
    free(scheduler_queue);
}

void free_scheduler_queues() {
    list_destroy_and_destroy_elements(scheduler_queues, (void (*)(void *)) free_dispatcher_queue);
}