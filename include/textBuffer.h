#pragma once
#include "buffer.h"

#include <stdbool.h>

struct textBuffer {
    struct buffer buffer;
    bool trackingLines;
};

int textBuffer_init(struct textBuffer *self, bool trackLines);

void textBuffer_toggleLineTracking(struct textBuffer *self);
static inline bool textBuffer_isTrackingLines(struct textBuffer *self) {
    return self->trackingLines;
}

void textBuffer_deinit(struct textBuffer *self);