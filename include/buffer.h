#pragma once
#include <inttypes.h>
#include <assert.h>

#define BUFFER_MEMORY_ALLOCATION_ERROR -1

struct buffer_modification {
    char *insert; // NULL if deletion, else the inserted text.
    int64_t textOffset; // Offset where the modification is made.
    int64_t delta; // The length delta of this modification. Negative if deletion, positive if insertion.
};

struct buffer_cursor {
    int64_t bufferOffset;
};

struct buffer {
    struct buffer_cursor cursor;
    const char *text;
    int64_t textLength;
    struct buffer_modification *modifications;
    int32_t numModifications;
};

int buffer_init(struct buffer *self, const char *text, int64_t textLength);
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