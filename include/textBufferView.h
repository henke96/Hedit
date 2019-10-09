#pragma once

#include "bufferView.h"

struct buffer;

struct textBufferView_cursor;

struct textBufferView {
    struct bufferView bufferView;
    struct textBufferView_cursor *cursors;
    struct buffer *buffer;
    int32_t numCursors;
};

void textBufferView_init(struct textBufferView *self, const char *text, int64_t textLength);
void textBufferView_deinit(struct textBufferView *self);
static inline struct bufferView *textBufferView_getBufferView(const struct textBufferView *self) {
    return &self->bufferView;
}

int textBufferView_createCursor(struct bufferView *self, int32_t *out_cursor);
void textBufferView_destroyCursor(struct bufferView *self, int32_t cursor);
void textBufferView_moveCursor(const struct bufferView *self, int32_t cursor, int64_t offset);
void textBufferView_copyCursor(struct bufferView *self, int32_t srcCursor, int32_t destCursor);
int textBufferView_insertAtCursor(struct bufferView *self, int32_t cursor, const char *str, int64_t strLength);
int textBufferView_deleteAtCursor(struct bufferView *self, int32_t cursor, int64_t length);

static inline int64_t textBufferView_getCursorLine(const struct bufferView *self, int32_t cursor) {
    return ((struct textBufferView *)self)->cursors[cursor].line;
}