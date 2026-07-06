#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "commands.h"

#define MAX_COMMAND_LENGTH 256

void shell_run(void) {
    char command[MAX_COMMAND_LENGTH];

    printf("=====================================\n");
    printf("       Filesystem shell\n");
    printf("=====================================\n");
    printf("Type 'help' to see available commands.\n\n");

    while (1) {
        printf("fs> ");

        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
            continue;

        if (strcmp(command, "exit") == 0)
            break;

        execute_command(command);        
    }

    printf("shell terminated.\n");
}