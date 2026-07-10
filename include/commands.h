#ifndef COMMANDS_H
#define COMMANDS_H

#include "fs.h"

void execute_command(const char *input);

extern FileSystem current_fs;

extern int filesystem_mounted;

extern int current_directory;



#endif