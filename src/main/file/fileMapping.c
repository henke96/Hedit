#include "main/file/fileMapping.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int fileMapping_init(struct fileMapping *self, const char *path) {
    FILE *handle = fopen(path, "rb");
    if (handle == NULL) return fileMapping_init_FILE_READING_ERROR;

    fseek(handle, 0, SEEK_END);
    self->contentSize = ftell(handle);
    rewind(handle);

    self->content = malloc(self->contentSize);
    if (self->content == NULL) return fileMapping_init_MEMORY_ALLOCATION_ERROR;

    int64_t readSize = (int64_t)fread(self->content, 1, self->contentSize, handle);

    if (self->contentSize != readSize) {
        free(self->content);
        fclose(handle);
        return fileMapping_init_FILE_READING_ERROR;
    }
    fclose(handle);
    return 0;
}

static void fileMapping_deinit(struct fileMapping *self) {
    free(self->content);
}
