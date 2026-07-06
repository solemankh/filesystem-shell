#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#include "fs.h"

int fs_format(const char *filename, size_t size)
{
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

    SuperBlock *sb = (SuperBlock *)base;
    sb->magic = FS_MAGIC;
    sb->fs_size = size;
    sb->block_size = FS_BLOCK_SIZE;
    sb->total_blocks = size / FS_BLOCK_SIZE;

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