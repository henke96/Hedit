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

static void buffer_init(struct buffer *self, const char *text, int64_t textLength);
static void buffer_deinit(struct buffer *self);

static int buffer_registerCursor(struct buffer *self, struct buffer_cursor *cursor);
static void buffer_unregisterCursor(struct buffer *self, struct buffer_cursor *cursor);

static void buffer_moveCursor(const struct buffer *self, struct buffer_cursor *cursor, int64_t offset);
static int buffer_insertAtCursor(struct buffer *self, const struct buffer_cursor *cursor, const char *str, int64_t strLength);
static int buffer_deleteAtCursor(struct buffer *self, const struct buffer_cursor *cursor, int64_t length);

static struct bufferChunk buffer_getCursorChunk(const struct buffer *self, const struct buffer_cursor *cursor);

#define buffer_GET_LENGTH(SELF) ((SELF).bufferLength)
#define buffer_MOVE_CURSOR_TO(SELF, CURSOR, BUFFER_OFFSET) buffer_moveCursor(&(SELF), &(CURSOR), (BUFFER_OFFSET) - cursor.bufferOffset) 

static inline char buffer_getAtCursor(const struct buffer *self, struct buffer_cursor *cursor) {
    if (cursor->offset < 0) {
        assert(-cursor->offset <= self->modifications[cursor->prevModificationIndex].insertLength);
        return self->modifications[cursor->prevModificationIndex].insertEnd[cursor->offset];
    } else {
        assert(self->textLength > cursor->offset);
        return self->text[cursor->offset];
    }
}

#define buffer_cursor_INIT_COPY(SELF, COPY_FROM) \
    (SELF) = (COPY_FROM)

static void buffer_cursor_init(struct buffer_cursor *self, const struct buffer *buffer);
#define buffer_cursor_DEINIT(SELF)

#define buffer_cursor_GET_OFFSET(SELF) ((SELF).bufferOffset)