#ifndef COMMANDS_H
#define COMMANDS_H

#include "fs.h"

void execute_command(const char *input);

extern FileSystem current_fs;

extern int filesystem_mounted;

#define CURRENT_DIRECTORY 0



#endif