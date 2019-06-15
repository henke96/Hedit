#pragma once
#include <inttypes.h>
#include <assert.h>

#define BUFFER_CHUNK_SIZE 0xFFFF

#define BUFFER_MEMORY_ALLOCATION_ERROR -1

struct buffer_chunk {
    char *text;
    int64_t length;
};

struct buffer_cursor {
    int32_t chunkIndex;
    int32_t chunkOffset;
};

struct buffer {
    struct buffer_cursor cursor;
    struct buffer_chunk *chunks;
    int32_t numChunks;
};

int buffer_init(struct buffer *self, const char *initialText, int64_t initialTextLength);
void buffer_deinit(struct buffer *self);

void buffer_setCursor(struct buffer *self, int64_t row, int64_t col);
void buffer_moveCursor(struct buffer *self, int64_t offset);
void buffer_insertBeforeCursor(struct buffer *self, char c);
void buffer_deleteAtCursor(struct buffer *self);

static inline char buffer_getAtCursor(const struct buffer *self) {
    assert(
        self->cursor.chunkIndex < self->numChunks &&
        self->cursor.chunkOffset < self->chunks[self->cursor.chunkIndex].length
    );
    return self->chunks[self->cursor.chunkIndex].text[self->cursor.chunkOffset];
}

static inline void buffer_setAtCursor(struct buffer *self, char c) {
    assert(
        self->cursor.chunkIndex < self->numChunks &&
        self->cursor.chunkOffset < self->chunks[self->cursor.chunkIndex].length
    );
    self->chunks[self->cursor.chunkIndex].text[self->cursor.chunkOffset] = c;
}