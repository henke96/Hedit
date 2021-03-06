#include "main/file/fileWriter.h"

static int fileWriter_open(struct fileWriter *self) {
    self->handle = fopen(self->path, "wb");
    if (self->handle == NULL) {
        return fileWriter_open_FILE_OPEN_ERROR;
    }
    return 0;
}

static int fileWriter_append(const struct fileWriter *self, const char *data, int64_t length) {
    int64_t writtenLength = (int64_t)fwrite(data, 1, length, self->handle);
    if (writtenLength != length) {
        return fileWriter_append_FILE_WRITE_ERROR;
    }
    return 0;
}

static int fileWriter_close(const struct fileWriter *self) {
    if (fclose(self->handle) == EOF) {
        return fileWriter_close_FILE_CLOSE_ERROR;
    }
    return 0;
}
