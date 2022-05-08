#include <bits/types/FILE.h>
#include "console_args_parser.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../../Utils/include/garbage_collector.h"

t_console_message* parse_program_args(char **argv) {

    FILE *fp = fopen(argv[2], "r");
    fseek(fp, 0, SEEK_END);
    uint32_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* instructions = safe_malloc(file_size + 1);
    consider_as_garbage(instructions, free);

    fread(instructions, file_size+1, 1, fp);
    fclose(fp);
    instructions[file_size] = '\0';

    t_console_message* console_message = safe_malloc(sizeof (t_console_message));
    consider_as_garbage(console_message, free);
    console_message -> process_size = atoi(argv[1]);
    console_message -> instructions = instructions;

    return console_message;
}