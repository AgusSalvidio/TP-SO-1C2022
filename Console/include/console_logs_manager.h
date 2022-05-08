#ifndef TP_2022_1C_LA_SCALONETA_CONSOLE_LOGS_MANAGER_H
#define TP_2022_1C_LA_SCALONETA_CONSOLE_LOGS_MANAGER_H

#include <stdint.h>

void initialize_console_logs_manager();

void free_console_logs_manager();

void log_invalid_transition_error();

void log_invalid_number_of_params(char* code, uint32_t expected, uint32_t actual);

#endif //TP_2022_1C_LA_SCALONETA_CONSOLE_LOGS_MANAGER_H
