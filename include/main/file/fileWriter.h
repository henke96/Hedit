#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <inttypes.h>
#include <stdio.h>

#define fileWriter_init_FILE_OPEN_ERROR -1
#define fileWriter_append_FILE_WRITE_ERROR -1
#define fileWriter_close_FILE_CLOSE_ERROR -1

struct fileWriter {
    const char *path;
    FILE *handle;
};

static inline void fileWriter_init(struct fileWriter *self, const char *path) {
    self->path = path;
}
static inline void fileWriter_deinit(struct fileWriter *self) {}

int fileWriter_open(struct fileWriter *self);
int fileWriter_append(const struct fileWriter *self, const char *data, int64_t length);
int fileWriter_close(const struct fileWriter *self);
