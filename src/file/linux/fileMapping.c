#include "file/fileMapping.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

int fileMapping_init(struct fileMapping *self, const char *path) {
    self->fd = open(path, O_RDONLY);
    if (self->fd < 0) {
        goto cleanup_0;
    }
    struct stat fileStat;
    if (fstat(self->fd, &fileStat) < 0) {
        goto cleanup_1;
    }
    self->contentSize = fileStat.st_size;

    self->content = mmap(NULL, self->contentSize, PROT_READ, MAP_PRIVATE, self->fd, 0);
    if (self->content == MAP_FAILED) {
        goto cleanup_1;
    }

    for (int i = 0; i < self->contentSize; ++i) {
        printf("%c", self->content[i]);
    }
    printf("\n");
    return 0;

    cleanup_1:
    close(self->fd);
    cleanup_0:
    return fileMapping_init_FILE_READING_ERROR;
}

void fileMapping_deinit(struct fileMapping *self) {
    munmap(self->content, self->contentSize);
    close(self->fd);
}
