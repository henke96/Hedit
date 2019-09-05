#pragma once
#include <inttypes.h>
#include <stdio.h>

#define fileWriter_init_FILE_OPEN_ERROR -1
#define fileWriter_append_FILE_WRITE_ERROR -1
#define fileWriter_close_FILE_CLOSE_ERROR -1

struct fileWriter {
    FILE *handle;
};

int fileWriter_init(struct fileWriter *self, const char *path);
int fileWriter_append(const struct fileWriter *self, char *data, int64_t length);
int fileWriter_close(const struct fileWriter *self);
static inline void fileWriter_deinit(struct fileWriter *self) {}
