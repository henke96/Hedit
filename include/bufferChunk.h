#pragma once
#include <inttypes.h>

struct bufferChunk {
    const char *text;
    int64_t length;
    int64_t cursorOffset;
};

static inline const char *bufferChunk_getText(const struct bufferChunk *self) {
    return self->text;
}

static inline int64_t bufferChunk_getLength(const struct bufferChunk *self) {
    return self->length;
}

static inline int64_t bufferChunk_getCursorOffset(const struct bufferChunk *self) {
    return self->cursorOffset;
}
