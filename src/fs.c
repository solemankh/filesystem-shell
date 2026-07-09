#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#include "fs.h"

int fs_format(const char *filename, size_t size) {

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (ftruncate(fd, size) < 0) {
        perror("ftruncate");
        close(fd);
        return -1;
    }

    void *base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (base == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    memset(base, 0, size);

    SuperBlock *sb = (SuperBlock *) base;
    sb->magic = FS_MAGIC;
    sb->fs_size = size;
    sb->block_size = FS_BLOCK_SIZE;
    sb->total_blocks = size / FS_BLOCK_SIZE;
    sb->directory_count = 1;
    sb->file_count = 0;

    DirectoryEntry *directories = fs_get_directory_table(base);
    FileEntry *files = fs_get_file_table(base);

    memset(directories, 0, MAX_DIRECTORIES * sizeof(DirectoryEntry));
    memset(files, 0, MAX_FILES * sizeof(FileEntry));

    strncpy(directories[0].name, "/", MAX_FILENAME_LENGTH - 1);
    directories[0].name[MAX_FILENAME_LENGTH - 1] = '\0';
    directories[0].parent = -1;
    directories[0].used = 1;

    if (msync(base, size, MS_SYNC) < 0) {
        perror("msync");
        munmap(base, size);
        close(fd);
        return -1;
    }

    munmap(base, size);
    close(fd);

    return 0;    
}

SuperBlock *fs_get_superblock(void *base) {
    return (SuperBlock *) base;
}

DirectoryEntry *fs_get_directory_table(void *base) {
    return (DirectoryEntry *)((char *)base + sizeof(SuperBlock));
}

FileEntry *fs_get_file_table(void *base) {
    return (FileEntry *)((char *)base + FILE_TABLE_OFFSET);
}

int fs_find_free_directory(void *base) {
    DirectoryEntry *directories = fs_get_directory_table(base);
    
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (!directories[i].used)
            return i;
    }

    return -1;
}

int fs_find_directory(void *base, const char *name, int parent) {
    DirectoryEntry *directories = fs_get_directory_table(base);

    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (directories[i].used && directories[i].parent == parent && strcmp(directories[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

int fs_find_free_file(void *base) {
    FileEntry *files = fs_get_file_table(base);

    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used)
            return i;
    }

    return -1;
}

int fs_find_file(void *base, const char *name, int parent) {
    FileEntry *files = fs_get_file_table(base);

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && files[i].parent == parent && strcmp(files[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

int fs_create_file(void *base, const char *name, int parent) {
    if (fs_find_file(base, name, parent) >= 0) {
        printf("File already exists.\n");
        return -1;
    }

    int index = fs_find_free_file(base);

    if (index == -1)
        return -1;

    FileEntry *files = fs_get_file_table(base);

    strncpy(files[index].name, name, MAX_FILENAME_LENGTH -1);
    files[index].name[MAX_FILENAME_LENGTH -1] = '\0';

    files[index].parent = parent;
    files[index].size = 0;
    files[index].offset = fs_find_free_offset(base);
    files[index].used = 1;

    SuperBlock *sb = fs_get_superblock(base);
    sb->file_count++;

    return index;    
}

int fs_create_directory(void *base, const char *name, int parent) {

    if (fs_find_directory(base, name, parent) >= 0) {
        printf("Directory already exists.\n");
        return -1;
    }
        


    int index = fs_find_free_directory(base);

    if (index == -1)
        return -1;

    DirectoryEntry *directories = fs_get_directory_table(base);

    strncpy(directories[index].name, name, MAX_FILENAME_LENGTH -1);
    directories[index].name[MAX_FILENAME_LENGTH -1] = '\0';

    directories[index].parent = parent;
    directories[index].used = 1;

    SuperBlock *sb = fs_get_superblock(base);
    sb->directory_count++;

    return index;    
}

int fs_mount(const char *filename, FileSystem *fs)
{
    int fd = open(filename, O_RDWR);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    SuperBlock sb;

    if (read(fd, &sb, sizeof(SuperBlock)) != sizeof(SuperBlock)) {
        perror("read");
        close(fd);
        return -1;
    }

    if (sb.magic != FS_MAGIC) {
        printf("Invalid filesystem.\n");
        close(fd);
        return -1;
    }

    void *base = mmap(NULL,
                      sb.fs_size,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      fd,
                      0);

    if (base == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    fs->base = base;
    fs->size = sb.fs_size;

    close(fd);

    return 0;
}

void fs_print_directories(void *base) {
    DirectoryEntry *directories = fs_get_directory_table(base);

    printf("\nDirectory Table\n");
    printf("-----------------------\n");

    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (directories[i].used) {
            printf("[%d]  %s  parent=%d\n", i, directories[i].name, directories[i].parent);
        }
    }

    printf("------------------------\n");
}

void fs_print_files(void *base) {
    FileEntry *files = fs_get_file_table(base);

    printf("\nFile Table\n");
    printf("---------------------------------\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            printf("[%d] %s parent=%d size=%zu\n", i, files[i].name, files[i].parent, files[i].size);

        }
    }

    printf("--------------------------------\n");
}

void fs_list_directory(void *base, int parent) {
    DirectoryEntry *directories = fs_get_directory_table(base);
    FileEntry *files = fs_get_file_table(base);

    printf("Directories:\n");

    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        if (directories[i].used && directories[i].parent == parent && strcmp(directories[i].name, "/") != 0) {
            printf(" %s/\n", directories[i].name);
        }
    }

    printf("\nFiles:\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && files[i].parent == parent) {
            printf( "%s\n", files[i].name);
        }
    }
}

size_t fs_find_free_offset(void *base) {
    FileEntry *files = fs_get_file_table(base);

    size_t max_offset = DATA_AREA_OFFSET;
    
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            size_t end = files[i].offset + files[i].size;

            if (end > max_offset)
                max_offset = end;
        }
    }

    return max_offset;
}

int fs_append_file(void *base, const char *name, int parent, const char *text) {
    int index = fs_find_file(base, name, parent);

    if (index < 0) {
        printf("File not found.\n");
        return -1;
    }

    FileEntry *files = fs_get_file_table(base);

    char *data_area = (char *)base + files[index].offset;

    strcpy(data_area + files[index].size, text);

    files[index].size += strlen(text);

    return 0;

}

int fs_cat_file(void *base, const char *name, int parent) {
    int index = fs_find_file(base, name, parent);

    if (index < 0) {
        printf("File not found.\n");
        return -1;
    }

    FileEntry *files = fs_get_file_table(base);

    char *data = (char *)base + files[index].offset;

    printf("%.*s\n", (int)files[index].size, data);

    return 0;
}

int fs_remove_file(void *base, const char *name, int parent) {
    int index = fs_find_file(base, name, parent);

    if (index < 0) {
        printf("File not found.\n");
        return -1;
    }

    FileEntry *files = fs_get_file_table(base);

    files[index].used = 0;
    files[index].size = 0;
    files[index].offset = 0;
    files[index].name[0] = '\0';
    files[index].parent = -1;

    SuperBlock *sb = fs_get_superblock(base);

    if (sb->file_count > 0)
        sb->file_count--;

    return 0;    

}