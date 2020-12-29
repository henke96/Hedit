#pragma once

#include "main/buffer.h"

struct ui {
    struct buffer *buffer;
    struct buffer_cursor lineCursor;

    char *inputBuffer;
    int64_t inputBufferCapacity;
};

static void ui_init(struct ui *self, struct buffer *buffer);

static int ui_run(struct ui *self);