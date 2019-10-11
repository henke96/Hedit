#pragma once
#include <inttypes.h>

#define bufferView_DEFAULT_CURSOR 0

enum bufferView_type {
    bufferView_type_TEXT,
    bufferView_type_HEX
};

struct bufferView {
    struct buffer *buffer;
    int64_t length;
    enum bufferView_type type;
};

#include "textBufferView.h"

static inline enum bufferView_type bufferView_getType(const struct bufferView *self) {
    return self->type;
}

static inline enum bufferView_type bufferView_getLength(const struct bufferView *self) {
    return self->length;
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
        case bufferView_type_TEXT: return textBufferView_insertAtCursor(self, cursor, str, strLength);
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}

static inline int bufferView_deleteAtCursor(struct bufferView *self, int32_t cursor, int64_t length) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_deleteAtCursor(self, cursor, length);
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}

static inline struct bufferChunk bufferView_getCursorChunk(const struct bufferView *self, int32_t cursor) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_getCursorChunk(self, cursor);
        case bufferView_type_HEX: return (struct bufferChunk) {0};
        default: HEDIT_UNREACHABLE;
    }
}

static inline int64_t bufferView_getCursorLine(const struct bufferView *self, int32_t cursor) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_getCursorLine(self, cursor);
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}

static inline int64_t bufferView_getCursorColumn(const struct bufferView *self, int32_t cursor) {
    switch (self->type) {
        case bufferView_type_TEXT: return textBufferView_getCursorColumn(self, cursor);
        case bufferView_type_HEX: return -1;
        default: HEDIT_UNREACHABLE;
    }
}
