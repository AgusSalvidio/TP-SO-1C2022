#include <malloc.h>
#include "kernel_srt_algorithm.h"
#include "../../Utils/include/garbage_collector.h"
#include "kernel_configuration.h"
#include "kernel_scheduler_queues.h"
#include "kernel_event.h"
#include "kernel_cpu_connection.h"

t_scheduling_algorithm *srt_algorithm;
t_list *pcbs_burst_estimations;

void calculate_current_estimation_for(t_burst_estimation *burst_estimation) {

    burst_estimation->pcb->next_burst =
            (burst_estimation->previous) * get_alpha() + (1 - get_alpha()) * (burst_estimation->previous_real);

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
        burst_estimation->previous = pcb->next_burst;
        burst_estimation->previous_real = 0;
        list_add(pcbs_burst_estimations, burst_estimation);
    }

    return burst_estimation;
}

void srt_update_ready_queue_when_adding_function(t_pcb *pcb) {
    t_burst_estimation *burst_estimation =
            burst_estimation_of_process(pcb);
    calculate_current_estimation_for(burst_estimation);

    bool _shortest_remaining_time(t_pcb *pcb_to_compare) {
        return pcb_to_compare->next_burst <= pcb->next_burst;
    }

    list_sort(scheduler_queue_of(READY)->pcb_list, _shortest_remaining_time);

    burst_estimation->previous_real = 0;
}

void update_previous(t_burst *burst) {
    t_burst_estimation *burst_estimation =
            burst_estimation_of_process(burst->pcb);
    //TODO revisar calculo estimacion.
}

void send_interruption_signal () {
    connect_and_send_interruption_to_cpu();
}

void srt_resolve_dependencies_function(t_burst *burst) {
    subscribe_to_event_doing(CONTEXT_SWITCH, (void (*)(void *)) update_previous);
    subscribe_to_event_doing(NEW_PROCESS_READY_TO_EXECUTE, send_interruption_signal);
    subscribe_to_event_doing(BLOCKED_PROCESS_READY_TO_EXECUTE, send_interruption_signal);
    subscribe_to_event_doing(SUSPENDED_PROCESS_READY_TO_EXECUTE, send_interruption_signal);
}

void free_burst_estimations() {
    list_destroy_and_destroy_elements(pcbs_burst_estimations, free);
}

void initialize_srt_algorithm() {
    srt_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    srt_algorithm->algorithm_name = "SRT";
    srt_algorithm->update_ready_queue_when_adding_function = srt_update_ready_queue_when_adding_function;
    srt_algorithm->resolve_dependencies_function = srt_resolve_dependencies_function;
    pcbs_burst_estimations = list_create();
    consider_as_garbage(pcbs_burst_estimations, free_burst_estimations);
}

t_scheduling_algorithm *get_srt_algorithm() {
    return srt_algorithm;
}