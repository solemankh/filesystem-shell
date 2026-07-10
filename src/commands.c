#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fs.h"
#include "commands.h"

FileSystem current_fs = {0};
int filesystem_mounted = 0;
int current_directory = 0;

void execute_command(const char *input) {
    if(strcmp(input, "help") == 0) {
        printf("Available commands:\n");
        printf(" format\n");
        printf(" mount\n");
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

    if (sscanf(input, "%63s %127s", cmd, filename) == 2) {
        if (strcmp(cmd, "mount") == 0) {
            if (fs_mount(filename, &current_fs) == 0) {
                filesystem_mounted = 1;
                printf("Filesystem mounted.\n");
            }else {
                printf("Unable to mount filesystem.\n");
            }
            return;
        }
    }

    char dirname[MAX_FILENAME_LENGTH];

    if (sscanf(input, "%63s %31s", cmd, dirname) == 2) {
        if (strcmp(cmd, "mkdir") == 0) {
            if (!filesystem_mounted) {
                printf("No filesystem mounted.\n");
                return;
            }
            int result = fs_create_directory(current_fs.base, dirname, current_directory);

            if (result >= 0)
                printf("Directory '%s' created.\n", dirname);
            else
                printf("Unable to create directory.\n");
            return;        
        }
    }

    char cd_directory[MAX_FILENAME_LENGTH];

    if (sscanf(input, "%63s %31s", cmd, cd_directory) == 2) {
        if (strcmp(cmd, "cd") == 0) {
            if (!filesystem_mounted) {
                printf("No filesystem mounted.\n");
                return;
            }

            int result = fs_change_directory(current_fs.base, cd_directory, current_directory);

            if (result >= 0) {
                current_directory = result;
                printf("Current directory: %s\n", cd_directory);
            } else {
                printf("Directory not found.\n");
            }

            return;
        }
    }

    char filename_touch[MAX_FILENAME_LENGTH];

    if (sscanf(input, "%63s %31s", cmd, filename_touch) == 2) {

        if (strcmp(cmd, "touch") == 0) {

            if (!filesystem_mounted) {
                printf("No filesystem mounted.\n");
                return;
            }

            int result = fs_create_file(current_fs.base,
                                        filename_touch,
                                        current_directory);

            if (result >= 0)
                printf("File '%s' created.\n", filename_touch);
            else
                printf("Unable to create file.\n");

            return;
        }
    }

    char append_filename[MAX_FILENAME_LENGTH];
    char append_text[256];

    if (sscanf(input, "%63s %31s %255[^\n]", cmd, append_filename, append_text) ==3) {

        if (strcmp(cmd, "append") == 0) {
            if (!filesystem_mounted) {
                printf("No filesystem mounted.\n");
                return;
            }

            int result = fs_append_file(current_fs.base, append_filename, current_directory, append_text);
            if (result == 0)
                printf("Text appended to '%s'.\n", append_filename);
            else
                printf("Unable to append text.\n");

            return;        
        }
    }

    char cat_filename[MAX_FILENAME_LENGTH];

    if (sscanf(input, "%63s %31s", cmd, cat_filename) ==2) {
        if (strcmp(cmd, "cat") == 0) {
            if (!filesystem_mounted) {
                printf("No filesystem mounted.\n");
                return;
            }

            if (fs_cat_file(current_fs.base, cat_filename, current_directory) != 0) {
                printf("Unable to read file.\n");
            }

            return;
        }
    }

    char rm_filename[MAX_FILENAME_LENGTH];
    if (sscanf(input, "%63s %31s", cmd, rm_filename) == 2) {
        if (strcmp(cmd, "rm") == 0) {
            if (!filesystem_mounted) {
                printf("No filesystem mounted.\n");
                return;
            }

            int result = fs_remove_file(current_fs.base, rm_filename, current_directory);

            if (result != 0) {
                result = fs_remove_directory(current_fs.base, rm_filename, current_directory);
            }

            if (result == 0)
                printf("'%s' removed.\n", rm_filename);
            else
                printf("Unable to remove '%s'.\n", rm_filename);

            return;         
        }
    }

    if (strcmp(input, "ls") == 0) {
        if (!filesystem_mounted) {
            printf("No filesystem mounted.\n");
            return;
        }

        fs_list_directory(current_fs.base, current_directory);
        return;
    }

    if (strcmp(input, "debug_dirs") == 0) {
        if (!filesystem_mounted) {
            printf("No filesystem mounted.\n");
            return;
        }

        fs_print_directories(current_fs.base);
        return;
    }

    if (strcmp(input, "debug_files") == 0) {
        if (!filesystem_mounted) {
            printf("No filesystem mounted.\n");
            return;
        }

        fs_print_files(current_fs.base);
        return;
    }



    printf("Unknown command: %s\n", input);
}
