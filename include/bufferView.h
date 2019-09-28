#pragma once
#include <inttypes.h>

enum bufferView_type {
    bufferView_type_TEXT,
    bufferView_type_HEX
};

struct bufferView_cursor {
    enum bufferView_type type;
};

static inline enum bufferView_type bufferView_cursor_getType(const struct bufferView *self) {
    return self->type;
}

struct bufferView {
    enum bufferView_type type;
};

static inline enum bufferView_type bufferView_getType(const struct bufferView *self) {
    return self->type;
}

static inline int bufferView_registerCursor(struct bufferView *self, struct bufferView_cursor *cursor) {
    switch (self->type) {
        case bufferView_type_TEXT:
            break;
        case bufferView_type_HEX:
            break;
        default:
            HEDIT_UNREACHABLE;
    }
    return 0;
}
