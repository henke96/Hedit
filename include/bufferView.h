#pragma once
#include <inttypes.h>

#define bufferView_DEFAULT_CURSOR 0

enum bufferView_type {
    bufferView_type_TEXT,
    bufferView_type_HEX
};

struct bufferView {
    enum bufferView_type type;
};

#include "textBufferView.h"

static inline enum bufferView_type bufferView_getType(const struct bufferView *self) {
    return self->type;
}

static inline int bufferView_createCursor(struct bufferView *self, int32_t *out_cursor) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_createCursor(self, out_cursor);
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}

static inline void bufferView_destroyCursor(struct bufferView *self, int32_t cursor) {
   switch (self->type) {
        case bufferView_type_TEXT: textBufferView_destroyCursor(self, cursor); break;
        case bufferView_type_HEX: break;
        default: HEDIT_UNREACHABLE;
    }
}

static inline void bufferView_copyCursor(struct bufferView *self, int32_t srcCursor, int32_t destCursor) {
   switch (self->type) {
        case bufferView_type_TEXT: textBufferView_copyCursor(self, srcCursor, destCursor); break;
        case bufferView_type_HEX: break;
        default: HEDIT_UNREACHABLE;
    }
}

static inline void bufferView_moveCursor(struct bufferView *self, int32_t cursor, int64_t offset) {
   switch (self->type) {
        case bufferView_type_TEXT: textBufferView_moveCursor(self, cursor, offset); break;
        case bufferView_type_HEX: break;
        default: HEDIT_UNREACHABLE;
    }
}

static inline int bufferView_insertAtCursor(struct bufferView *self, int32_t cursor, const char *str, int64_t strLength) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_insertAtCursor(self, cursor, str, strLength); break;
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}
static inline int bufferView_deleteAtCursor(struct bufferView *self, int32_t cursor, int64_t length) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_deleteAtCursor(self, cursor, length); break;
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}
