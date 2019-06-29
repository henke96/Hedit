#pragma once
#include <inttypes.h>
#include <assert.h>

#define BUFFER_MEMORY_ALLOCATION_ERROR -1

struct buffer_modification {
    int64_t intervalStart;
    int64_t intervalEnd;
    int64_t insertLength;
    char *insertEnd;
};

struct buffer_cursor {
    int64_t offset;
    int32_t prevModificationIndex; // -1 if none.
};

struct buffer {
    struct buffer_cursor cursor;
    const char *text;
    int64_t textLength;
    struct buffer_modification *modifications;
    int32_t numModifications;
    int64_t bufferLength;
};

void buffer_init(struct buffer *self, const char *text, int64_t textLength);
void buffer_deinit(struct buffer *self);

void buffer_moveCursor(struct buffer *self, int64_t offset);
int buffer_insertAtCursor(struct buffer *self, const char *str, int64_t strLength);
void buffer_deleteAtCursor(struct buffer *self, int64_t length);

static inline char buffer_cursorNext(struct buffer *self) {
    ++self->cursor.offset;
    if (self->cursor.offset > 0) {
        int32_t next = self->cursor.prevModificationIndex + 1;
        if (
            next < self->numModifications &&
            self->cursor.offset >= self->modifications[next].intervalStart
        ) {
            self->cursor.prevModificationIndex = next;
            if (self->modifications[next].insertLength == 0) {
                self->cursor.offset = self->modifications[next].intervalEnd;
            } else {
                self->cursor.offset = -self->modifications[next].insertLength;
                return self->modifications[next].insertEnd[self->cursor.offset];
            }
        } 
    } else if (self->cursor.offset < 0) {
        return self->modifications[self->cursor.prevModificationIndex].insertEnd[self->cursor.offset];
    } else {
        self->cursor.offset = self->modifications[self->cursor.prevModificationIndex].intervalEnd;
    }
    return self->text[self->cursor.offset];
}

static inline char buffer_getAtCursor(const struct buffer *self) {
    if (self->cursor.offset < 0) {
        return self->modifications[self->cursor.prevModificationIndex].insertEnd[self->cursor.offset];
    } else {
        return self->text[self->cursor.offset];
    }
}