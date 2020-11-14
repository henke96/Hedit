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

#define fileWriter_INIT(SELF, PATH) \
    (SELF).path = (PATH)

#define fileWriter_DEINIT(SELF)

static int fileWriter_open(struct fileWriter *self);
static int fileWriter_append(const struct fileWriter *self, const char *data, int64_t length);
static int fileWriter_close(const struct fileWriter *self);
