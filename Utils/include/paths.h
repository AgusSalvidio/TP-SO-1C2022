#ifndef KISS_ROOTS_H
#define KISS_ROOTS_H

#endif //KISS_ROOTS_H

char* absolute_path_for_config();

char* logs_path();

char* get_file_name_from_path(char* path);

char* absolute_path_for_log_named(char* log_name);

bool exists_file_at(char* path);