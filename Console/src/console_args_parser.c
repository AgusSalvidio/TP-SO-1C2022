#include <bits/types/FILE.h>
#include "console_args_parser.h"
#include <stdio.h>
#include <commons/string.h>
#include <stdint.h>
#include <stdlib.h>
#include "../../Utils/include/logger.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"

char *parse_program_args(char **argv) {


    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    uint32_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* instructions = safe_malloc(file_size + 1);
    consider_as_garbage(instructions, free);

    fread(instructions, file_size+1, 1, fp);
    fclose(fp);
    instructions[file_size] = '\0';

    return instructions;
}