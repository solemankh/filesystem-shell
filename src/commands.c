#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fs.h"
#include "commands.h"

void execute_command(const char *input) {
    if(strcmp(input, "help") == 0) {
        printf("Available commands:\n");
        printf(" format\n");
        printf(" mkdir\n");
        printf(" cd\n");
        printf(" touch\n");
        printf(" cat\n");
        printf(" ls\n");
        printf(" append\n");
        printf(" rm\n");
        printf(" close\n");
        printf(" exit\n");

        return;
    }

    char cmd[64];
    char filename[128];
    size_t size;

    if (sscanf(input, "%63s %127s %zu", cmd, filename, &size) ==3) {
        if(strcmp(cmd, "format") == 0) {
            if (fs_format(filename, size) == 0)
                printf("Filesystem formatted: %s (%zu bytes)\n", filename, size);
            else
                printf("Error formatting filesystem\n");

            return;        
        }
    }

    printf("Unknown command: %s\n", input);
}
