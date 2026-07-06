#ifndef FS_H
#define FH_H

#include <stddef.h>

/*
 * Main structure representing the mounted filesystem.
 * It will contain the pointer returned by mmap()
 * and the filesystem size.
*/ 

typedef struct FileSystem {
    void *base;         //Start of the mapped memory
    unsigned int size;  // Filesystem size in bytes
} FileSystem;

#endif