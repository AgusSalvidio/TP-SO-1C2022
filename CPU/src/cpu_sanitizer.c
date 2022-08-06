#include <cpu_logs_manager.h>
#include <cpu_sanitizer.h>
#include <cpu_configuration_manager.h>
#include <cpu_serializable_objects.h>
#include <cpu_query_performer.h>
#include <cpu_manager.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"
#include "cpu_tlb.h"
#include "cpu_kernel_dispatch_connection_handler.h"
#include "cpu_kernel_interrupt_connection_handler.h"
#include "cpu_action_performer.h"

void free_system(){

    printf("1-");
    free_interrupt_thread();
    printf("2-");
    free_dispatch_thread();
    printf("3-");
    free_pretty_printer();
    printf("4-");
    free_cpu_serializable_objects();
    printf("5-");
    free_cpu_query_performers();
    printf("6-");
    free_cpu_manager();
    printf("7-");
    free_tlb();
    printf("8-");
    free_cpu_configuration_manager();
    printf("9-");
    free_action_performers();
    printf("10-");
    free_garbage_collector();
    printf("11-");
    log_successful_clean_up();
    printf("12-");
    free_cpu_logs_manager();
    printf("13-");
}