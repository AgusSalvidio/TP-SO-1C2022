#ifndef KISS_CONFIGURATION_MANAGER_H
#define KISS_CONFIGURATION_MANAGER_H

#include <commons/collections/list.h>

void initialize_configuration_manager();
void initialize_configuration_manager_from_file(char* config_file);

int config_get_int_at(char* key);
char* config_get_string_at(char* key);
char** config_get_char_array_at(char* key);
double config_get_double_at(char* key);
t_list* all_config_values();

void free_configuration_manager();
#endif //KISS_CONFIGURATION_MANAGER_H
