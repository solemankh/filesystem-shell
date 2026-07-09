#ifndef FS_H
#define FS_H

#include <stddef.h>

#define FS_MAGIC 0x20250706
#define FS_BLOCK_SIZE 4096

#define MAX_DIRECTORIES 128
#define MAX_FILES 256

#define MAX_FILENAME_LENGTH 32


/* ========================
 * Filesystem metadata
 * ========================*/

typedef struct {
    unsigned int magic;
    size_t fs_size;
    size_t block_size;
    size_t total_blocks;
    size_t directory_count;
    size_t file_count;
} SuperBlock;

/* ========================
 * Directory entyr
 * ========================*/

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    int parent;
    int used;
} DirectoryEntry;

/* ========================
 * File entry
 * ========================*/

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    int parent;
    size_t size;
    size_t offset;
    int used;
} FileEntry;

/* ========================
 * Mounted filesystem
 * ========================*/

typedef struct {
    void *base;         //Start of the mapped memory
    size_t size;  // Filesystem size in bytes
} FileSystem;

#define DIRECTORY_TABLE_OFFSET   sizeof(SuperBlock)

#define FILE_TABLE_OFFSET \
    (DIRECTORY_TABLE_OFFSET + \
     MAX_DIRECTORIES * sizeof(DirectoryEntry))

#define DATA_AREA_OFFSET \
    (FILE_TABLE_OFFSET + \
     MAX_FILES * sizeof(FileEntry))

/* ========================
 * Filesystem API
 * ========================*/

int fs_format(const char *filename, size_t size);

SuperBlock *fs_get_superblock(void *base);

DirectoryEntry *fs_get_directory_table(void *base);

FileEntry *fs_get_file_table(void *base);

int fs_find_free_directory(void *base);

int fs_find_directory(void *base, const char *name, int parent);

int fs_find_free_file(void *base);

int fs_find_file(void *base, const char *name, int parent);

int fs_create_file(void *base, const char *name, int parent);

int fs_append_file(void *base, const char *name, int parent, const char *text);

int fs_cat_file(void *base, const char *name, int parent);

int fs_remove_file(void *base, const char *name, int parent);

size_t fs_find_free_offset(void *base);

int fs_create_directory(void *base, const char *name, int parent);

int fs_mount(const char *filename, FileSystem *fs);

void fs_print_directories(void *base);

void fs_print_files(void *base);

void fs_list_directory(void *base, int parent);

#endif