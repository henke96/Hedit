#pragma once
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

static inline int64_t buffer_getLength(const struct buffer *self) {
    return self->bufferLength;
}

static inline void buffer_moveCursorTo(struct buffer *self, struct buffer_cursor *cursor, int64_t bufferOffset) {
    buffer_moveCursor(self, cursor, bufferOffset - cursor->bufferOffset);
}

static inline char buffer_cursorNext(const struct buffer *self, struct buffer_cursor *cursor) {
    ++cursor->bufferOffset;
    assert(cursor->bufferOffset <= self->bufferLength);
    ++cursor->offset;
    if (cursor->offset >= 0) {
        while (1) {
            if (cursor->offset == 0) {
                cursor->offset = self->modifications[cursor->prevModificationIndex].intervalEnd;
            }
            int32_t next = cursor->prevModificationIndex + 1;
            if (
                next < self->numModifications &&
                cursor->offset >= self->modifications[next].intervalStart
            ) {
                cursor->prevModificationIndex = next;
                cursor->offset = -self->modifications[next].insertLength;
                if (cursor->offset != 0) {
                    assert(cursor->offset < 0);
                    return self->modifications[next].insertEnd[cursor->offset];
                }
            } else {
                assert(cursor->offset >= 0);
                return self->text[cursor->offset];
            }
        }
    } else {
        return self->modifications[cursor->prevModificationIndex].insertEnd[cursor->offset];
    }
}

static inline char buffer_getAtCursor(const struct buffer *self, struct buffer_cursor *cursor) {
    if (cursor->offset < 0) {
        return self->modifications[cursor->prevModificationIndex].insertEnd[cursor->offset];
    } else {
        return self->text[cursor->offset];
    }
}

static inline void buffer_cursor_initCopy(struct buffer_cursor *self, const struct buffer_cursor *copyFrom) {
    *self = *copyFrom;
}

static inline void buffer_cursor_init(struct buffer_cursor *self, const struct buffer *buffer) {
    self->bufferOffset = 0;
    self->offset = 0;
    self->prevModificationIndex = -1;
    buffer_moveCursor(buffer, self, 0); // Fix offset incase there's a modification at the start.
}

static inline int64_t buffer_cursor_getOffset(const struct buffer_cursor *self) {
    return self->bufferOffset;
}