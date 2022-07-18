#include <malloc.h>
#include <commons/string.h>
#include <commons/log.h>
#include "kernel_srt_algorithm.h"
#include "../../Utils/include/garbage_collector.h"
#include "kernel_configuration.h"
#include "kernel_scheduler_queues.h"
#include "kernel_event.h"
#include "kernel_cpu_connection.h"
#include "../../Utils/include/logger.h"

t_scheduling_algorithm *srt_algorithm;
t_list *pcbs_burst_estimations;


bool shortest_remaining_time(t_pcb *p1, t_pcb *p2) {
    return p1->next_burst <= p2->next_burst;
}

t_burst_estimation *burst_estimation_of_process(t_pcb *pcb) {

    bool _is_for(t_burst_estimation *burst_estimation_to_compare) {
        return burst_estimation_to_compare->pcb == pcb;
    }

    t_burst_estimation *burst_estimation =
            list_find(pcbs_burst_estimations, (bool (*)(void *)) _is_for);

    if (!burst_estimation) {
        burst_estimation = safe_malloc(sizeof(t_burst_estimation));
        burst_estimation->pcb = pcb;
        burst_estimation->estimated_previous = pcb->next_burst;
        burst_estimation->previous_real = 0;
        list_add(pcbs_burst_estimations, burst_estimation);
    }

    return burst_estimation;
}

void srt_update_ready_queue_when_adding_function() {

    char* to_log = string_new();
    void _print_estimation(t_pcb* pcb) {
        string_append(&to_log, string_from_format("%d: %f|", pcb->pid, pcb->next_burst));
    };

    list_sort(scheduler_queue_of(READY)->pcb_list, (bool (*)(void *, void *)) shortest_remaining_time);
    list_iterate(scheduler_queue_of(READY)->pcb_list, _print_estimation);
    log_info(process_execution_logger(), to_log);

}

void update_previous(t_burst *burst) { //est * α + real * (1 - α)
    t_burst_estimation *burst_estimation =
            burst_estimation_of_process(burst->pcb);
    uint32_t last_burst = burst->finished - burst->start;
    //actualizo estimacion pcb
    burst_estimation -> pcb -> next_burst = burst_estimation -> pcb -> next_burst * get_alpha() + last_burst * (1-get_alpha());
    srt_update_ready_queue_when_adding_function();
}

void send_interruption_signal () {
    connect_and_send_interruption_to_cpu();
}

void srt_resolve_dependencies_function() {
    subscribe_to_event_doing(PROCESS_SWITCH, (void (*)(void *)) update_previous);
    subscribe_to_event_doing(SEND_INTERRUPTION_SIGNAL, send_interruption_signal);
}

void free_burst_estimations() {
    list_destroy_and_destroy_elements(pcbs_burst_estimations, free);
}

void initialize_srt_algorithm() {
    srt_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    srt_algorithm->algorithm_name = "SRT";
    srt_algorithm->resolve_dependencies_function = srt_resolve_dependencies_function;
    pcbs_burst_estimations = list_create();
    consider_as_garbage(pcbs_burst_estimations, free_burst_estimations);
}

t_scheduling_algorithm *get_srt_algorithm() {
    return srt_algorithm;
}