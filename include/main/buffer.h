#pragma once
#include "bufferChunk.h"

#include <inttypes.h>
#include <assert.h>

#define buffer_MEMORY_ALLOCATION_ERROR -1

struct buffer_modification {
    int64_t intervalStart;
    int64_t intervalEnd;
    int64_t insertLength;
    char *insertEnd;
};

struct buffer_cursor {
    int64_t bufferOffset;
    int64_t offset;
    int32_t prevModificationIndex; // -1 if none.
};

struct buffer {
    const char *text;
    int64_t textLength;
    int64_t bufferLength;
    struct buffer_modification *modifications;
    struct buffer_cursor **registeredCursors;
    int32_t numModifications;
    int32_t registeredCursorsCapacity;
};

void buffer_init(struct buffer *self, const char *text, int64_t textLength);
void buffer_deinit(struct buffer *self);

int buffer_registerCursor(struct buffer *self, struct buffer_cursor *cursor);
void buffer_unregisterCursor(struct buffer *self, struct buffer_cursor *cursor);

void buffer_moveCursor(const struct buffer *self, struct buffer_cursor *cursor, int64_t offset);
int buffer_insertAtCursor(struct buffer *self, const struct buffer_cursor *cursor, const char *str, int64_t strLength);
int buffer_deleteAtCursor(struct buffer *self, const struct buffer_cursor *cursor, int64_t length);

struct bufferChunk buffer_getCursorChunk(const struct buffer *self, const struct buffer_cursor *cursor);

static inline int64_t buffer_getLength(const struct buffer *self) {
    return self->bufferLength;
}

static inline void buffer_moveCursorTo(struct buffer *self, struct buffer_cursor *cursor, int64_t bufferOffset) {
    buffer_moveCursor(self, cursor, bufferOffset - cursor->bufferOffset);
}

static inline char buffer_getAtCursor(const struct buffer *self, struct buffer_cursor *cursor) {
    if (cursor->offset < 0) {
        assert(-cursor->offset <= self->modifications[cursor->prevModificationIndex].insertLength);
        return self->modifications[cursor->prevModificationIndex].insertEnd[cursor->offset];
    } else {
        assert(self->textLength > cursor->offset);
        return self->text[cursor->offset];
    }
}

static inline void buffer_cursor_init_copy(struct buffer_cursor *self, const struct buffer_cursor *copyFrom) {
    *self = *copyFrom;
}

void buffer_cursor_init(struct buffer_cursor *self, const struct buffer *buffer);
static inline void buffer_cursor_deinit(struct buffer_cursor *self) {}

static inline int64_t buffer_cursor_getOffset(const struct buffer_cursor *self) {
    return self->bufferOffset;
}
