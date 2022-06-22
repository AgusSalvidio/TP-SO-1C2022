#include <cpu_logs_manager.h>
#include <cpu_sanitizer.h>
#include <cpu_configuration_manager.h>
#include <cpu_serializable_objects.h>
#include <cpu_query_performer.h>
#include <cpu_manager.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"

void free_system(){

    free_pretty_printer();

    free_cpu_serializable_objects();
    free_cpu_query_performers();
    free_cpu_manager();
    free_garbage_collector();
    free_cpu_configuration_manager();
    log_successful_clean_up();
    free_cpu_logs_manager();
}