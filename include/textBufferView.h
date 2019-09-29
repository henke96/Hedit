#pragma once

#include "bufferView.h"

struct textBufferView {
    struct bufferView bufferView;
};

void textBufferView_init(struct textBufferView *self, const char *text, int64_t textLength);
void textBufferView_deinit(struct textBufferView *self);
static inline struct bufferView *textBufferView_getBufferView(const struct textBufferView *self) {
    return &self->bufferView;
}

int textBufferView_createCursor(struct bufferView *self, int32_t *out_cursor);
void bufferView_destroyCursor(struct bufferView *self, int32_t cursor);
void bufferView_copyCursor(struct bufferView *self, int32_t srcCursor, int32_t destCursor);
void bufferView_moveCursor(const struct bufferView *self, int32_t cursor, int64_t offset);
int bufferView_insertAtCursor(struct bufferView *self, int32_t cursor, const char *str, int64_t strLength);
int bufferView_deleteAtCursor(struct bufferView *self, int32_t cursor, int64_t length);