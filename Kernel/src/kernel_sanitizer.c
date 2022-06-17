#include "../include/kernel_sanitizer.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "../include/kernel_logs_manager.h"
#include "kernel_scheduler_queues.h"
#include "kernel_state_transitions.h"
#include "planification.h"
#include "kernel_long_term_scheduler.h"
#include "kernel_short_term_scheduler.h"
#include "kernel_event.h"
#include "kernel_io_routine.h"
#include "kernel_mid_term_scheduler.h"
#include "kernel_cpu_message_handler.h"

void free_system() {

    free_planificacion_threads();
    free_long_term_scheduler();
    free_mid_term_scheduler();
    free_short_term_scheduler();
    free_cpu_message_handler();
    free_scheduler_queues();
    free_state_transitions();
    free_io_routine();
    free_kernel_process_image();
    free_event_notifier();
    free_pretty_printer();
    free_serializable_objects();
    free_garbage_collector();
    free_configuration_manager();


    log_successful_clean_up();
    free_kernel_logs_manager();
}