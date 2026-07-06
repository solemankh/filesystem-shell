#include <stdio.h>
#include <string.h>
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

    printf("Unknown command: %s\n", input);
}
