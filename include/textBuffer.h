#pragma once
#include "buffer.h"
#include <stdbool.h>

struct textBuffer_cursor {
    struct buffer_cursor cursor;
    int64_t currentLine;
};

struct textBuffer {
    struct buffer buffer;
    bool trackingLines;
};

void textBuffer_init(struct textBuffer *self, const char *text, int64_t textLength, bool trackLines);

static inline void textBuffer_toggleLineTracking(struct textBuffer *self) {
    self->trackingLines = !self->trackingLines;
}
static inline bool textBuffer_isTrackingLines(struct textBuffer *self) {
    return self->trackingLines;
}

int textBuffer_registerCursor(struct textBuffer *self, struct textBuffer_cursor *cursor);
void textBuffer_unregisterCursor(struct textBuffer *self, struct textBuffer_cursor *cursor);

void textBuffer_deinit(struct textBuffer *self);

void textBuffer_cursor_init(struct textBuffer_cursor *self, const struct textBuffer *buffer);
void textBuffer_cursor_deinit(struct textBuffer_cursor *self) {}