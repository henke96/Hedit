#pragma once
#include <inttypes.h>

struct bufferView_cursor;

typedef int64_t (*bufferView_cursor_getOffset_t)(const struct bufferView_cursor *self);
typedef int64_t (*bufferView_cursor_getLine_t)(const struct bufferView_cursor *self);

struct bufferView_cursor {
    bufferView_cursor_getOffset_t getOffset;
    bufferView_cursor_getLine_t getLine;
};

static inline void bufferView_cursor_getOffset(const struct bufferView_cursor *self) {
    return self->getOffset(self);
}

static inline void bufferView_cursor_getLine(const struct bufferView_cursor *self) {
    return self->getLine(self);
}

struct bufferView;

typedef void (*bufferView_cursor_init_t)(struct bufferView_cursor *self, const struct bufferView *buffer);
typedef int (*bufferView_registerCursor_t)(struct bufferView *self, struct bufferView_cursor *cursor);
typedef void (*bufferView_unregisterCursor_t)(struct bufferView *self, struct bufferView_cursor *cursor);

struct bufferView {
    bufferView_cursor_init_t cursor_init;
    bufferView_registerCursor_t registerCursor;
    bufferView_unregisterCursor_t unregisterCursor;
};

static inline void bufferView_cursor_init(struct bufferView_cursor *self, const struct bufferView *buffer) {
    return buffer->cursor_init(self, buffer);
}

static inline int bufferView_registerCursor(struct bufferView *self, struct bufferView_cursor *cursor) {
    return self->registerCursor(self, cursor);
}

static inline void bufferView_unregisterCursor(struct bufferView *self, struct bufferView_cursor *cursor) {
    return self->unregisterCursor(self, cursor);
}
