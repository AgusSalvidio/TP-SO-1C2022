#include "console_configuration.h"
#include "../../Utils/include/configuration_manager.h"

char* get_kernel_ip() {
    return config_get_string_at("IP_KERNEL");
}

char* get_kernel_port() {
    return config_get_string_at("PUERTO_KERNEL");

}