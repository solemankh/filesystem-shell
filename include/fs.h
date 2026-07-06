#ifndef FS_H
#define FH_H

#include <stddef.h>

#define FS_MAGIC 0x20250706
#define FS_BLOCK_SIZE 4096

/*
 * Main structure representing the mounted filesystem.
 * It will contain the pointer returned by mmap()
 * and the filesystem size.
*/ 

typedef struct {
    unsigned int magic;
    size_t fs_size;
    size_t block_size;
    size_t total_blocks;
} SuperBlock;

typedef struct FileSystem {
    void *base;         //Start of the mapped memory
    unsigned int size;  // Filesystem size in bytes
} FileSystem;

int fs_format(const char *filename, size_t size);

#endif