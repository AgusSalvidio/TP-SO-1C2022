#include <memory_connection_handler.h>
#include <memory_logs_manager.h>
#include <memory_configuration_manager.h>
#include <memory_query_performers.h>
#include <memory_serializable_objects.h>
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"
#include "memory_replacement_algorithms.h"

void free_system(){


    free_memory_serializable_objects();

    free_memory_query_performers();

    free_memory_manager();

    free_memory_replacement_algorithms();

    free_garbage_collector();

    free_memory_configuration_manager();

    log_successful_clean_up();

    free_memory_logs_manager();

    free_main_thread();

}

