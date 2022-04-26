#include "console_sanitizer.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "console_logs_manager.h"

void free_system() {

    free_pretty_printer();
    free_serializable_objects();
    free_garbage_collector();
    free_configuration_manager();

    log_successful_clean_up();
    free_console_logs_manager();
}
