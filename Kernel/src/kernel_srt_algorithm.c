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
#include "kernel_logs_manager.h"
#include "../../Utils/include/pthread_wrapper.h"

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
        burst_estimation->previous_real = get_initial_estimation();
        burst_estimation->current = 0;
        list_add(pcbs_burst_estimations, burst_estimation);
    }

    return burst_estimation;
}

void srt_update_ready_queue_when_adding_function() {

    list_sort(scheduler_queue_of(READY)->pcb_list, (bool (*)(void *, void *)) shortest_remaining_time);

}

void update_current(t_burst *burst) {

    t_burst_estimation *burst_estimation = burst_estimation_of_process(burst->pcb);

    uint32_t last_burst = burst->finished - burst->start;

    burst_estimation -> current += last_burst;
    burst_estimation -> previous_real = burst_estimation -> current;
    double estimated = burst_estimation->pcb->next_burst;
    burst_estimation->pcb->next_burst -= last_burst;
    log_update_current_estimation(burst_estimation->pcb, estimated, burst_estimation->pcb->next_burst);
    srt_update_ready_queue_when_adding_function();
}

void send_interruption_signal () {
    if (!list_is_empty(scheduler_queue_of(EXEC)->pcb_list)) {
        connect_and_send_interruption_to_cpu();
    }
}

void calculate_estimation(t_pcb *pcb) { //est * α + real * (1 - α)

    t_burst_estimation *burst_estimation = burst_estimation_of_process(pcb);

    double previous_est = burst_estimation->pcb->next_burst + burst_estimation->current;

    burst_estimation -> pcb -> next_burst = previous_est * get_alpha() + burst_estimation -> previous_real * (1 - get_alpha());

    log_calculate_estimation(burst_estimation->pcb, previous_est, burst_estimation-> previous_real);
    burst_estimation -> current = 0;

    srt_update_ready_queue_when_adding_function();
}

void free_burst_estimation_of_process(t_pcb* pcb) {
    t_burst_estimation* estimator = burst_estimation_of_process(pcb);
    bool _is_for(t_burst_estimation *burst_estimation_to_compare) {
        return burst_estimation_to_compare->pcb == pcb;
    }

    list_remove_and_destroy_by_condition(pcbs_burst_estimations, (bool (*)(void *)) _is_for, free);

}

void srt_resolve_dependencies_function() {
    subscribe_to_event_doing(UPDATE_CURRENT_PROCESS_ESTIMATION, (void (*)(void *)) update_current);
    subscribe_to_event_doing(SEND_INTERRUPTION_SIGNAL, send_interruption_signal);
    subscribe_to_event_doing(CALCULATE_ESTIMATION_OF_PROCESS, (void (*)(void *)) calculate_estimation);
    subscribe_to_event_doing(FREE_PROCESS_ESTIMATION, (void (*)(void *)) free_burst_estimation_of_process);
}



void free_burst_estimations() {
    list_destroy_and_destroy_elements(pcbs_burst_estimations, free);
}

void initialize_srt_algorithm() {
    srt_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    srt_algorithm->algorithm_name = "SRT";
    srt_algorithm->resolve_dependencies_function = srt_resolve_dependencies_function;
    srt_algorithm->list_logger = log_estimation_list;
    pcbs_burst_estimations = list_create();
    consider_as_garbage(pcbs_burst_estimations, free_burst_estimations);
}

t_scheduling_algorithm *get_srt_algorithm() {
    return srt_algorithm;
}