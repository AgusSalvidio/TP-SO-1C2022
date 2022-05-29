#include <bits/types/FILE.h>
#include "console_args_parser.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../../Utils/include/garbage_collector.h"
#include <commons/string.h>
#include "console_instruction_handler.h"

t_list *parse_instructions(char *instructions_string) {
    t_list *instructions = list_create();
    char **instructions_array = string_split(instructions_string, "\n");
    for (int i = 0; instructions_array[i]; ++i) {
        char **instruction_str = string_split(instructions_array[i], " ");
        list_add(instructions, create_instruction_from(instruction_str));
        free_char_array(instruction_str);
    }
    free_char_array(instructions_array);
    return instructions;
}

t_console_message* parse_program_args(char **argv) {

    FILE *fp = fopen(argv[2], "r");
    fseek(fp, 0, SEEK_END);
    uint32_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* instructions_str = safe_malloc(file_size + 1);
    consider_as_garbage(instructions_str, free);

    fread(instructions_str, file_size + 1, 1, fp);
    fclose(fp);
    instructions_str[file_size] = '\0';

    t_console_message* console_message = safe_malloc(sizeof (t_console_message));
    consider_as_garbage(console_message, free);
    console_message -> process_size = atoi(argv[1]);
    console_message -> instructions = parse_instructions(instructions_str);
    stop_considering_garbage(instructions_str);
    free(instructions_str);
    return console_message;
}