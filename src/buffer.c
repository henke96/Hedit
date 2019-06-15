#include "buffer.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static inline int buffer_chunk_init(
    struct buffer_chunk *self,
    const char *text,
    int64_t length
) {
    assert(length >= 0);

    self->text = malloc(length);
    if (!self->text) return -1;
    memcpy(self->text, text, length);
    self->length = length;
    return 0;
}

static inline void buffer_chunk_deinit(struct buffer_chunk *self) {
    free(self->text);
}

static inline void buffer_chunk_deleteAt(struct buffer_chunk *self, int64_t offset) {
    --self->length;
    memmove(&self->text[offset], &self->text[offset + 1], self->length - offset);
}

static inline void buffer_cursor_init(
    struct buffer_cursor *self,
    int32_t chunkIndex,
    int32_t chunkOffset
) {
    assert(chunkIndex >= 0 && chunkOffset >= 0);

    self->chunkIndex = chunkIndex;
    self->chunkOffset = chunkOffset;
}

static inline int buffer_cursorInValidState(struct buffer *self) {
    struct buffer_cursor cursor = self->cursor;
    return (
        cursor.chunkIndex >= 0 && (
            cursor.chunkIndex < self->numChunks || (
                cursor.chunkIndex == self->numChunks &&
                cursor.chunkOffset == 0
            )
        )
    );
}

static inline void buffer_deleteChunk(struct buffer *self, int32_t chunkIndex) {
    buffer_chunk_deinit(&self->chunks[chunkIndex]);
    --self->numChunks;
    memmove(
        &self->chunks[chunkIndex],
        &self->chunks[chunkIndex + 1],
        (self->numChunks - chunkIndex) * sizeof(self->chunks[0])
    );
}

int buffer_init(struct buffer *self, const char *initialText, int64_t initialTextLength) {
    assert(initialTextLength >= 0);
    
    self->numChunks = (initialTextLength + BUFFER_CHUNK_SIZE - 1) / BUFFER_CHUNK_SIZE; // Round up.

    self->chunks = malloc(self->numChunks * sizeof(self->chunks[0]));
    if (!self->chunks) return BUFFER_MEMORY_ALLOCATION_ERROR;

    // Spread the text out over the chunks.
    int64_t textOffset = 0;
    for (int32_t i = 0; i < self->numChunks; ++i) {
        const int64_t remainingTextLength = initialTextLength - textOffset;
        const int32_t remainingChunks = self->numChunks - i;
        const int32_t chunkLength = remainingTextLength / remainingChunks;

        if (buffer_chunk_init(&self->chunks[i], &initialText[textOffset], chunkLength) < 0) {
            // Clean up if a chunk allocation fails.
            while (i--) {
                buffer_chunk_deinit(&self->chunks[i]);
            }
            free(self->chunks);
            return BUFFER_MEMORY_ALLOCATION_ERROR;
        }
        textOffset += chunkLength;
    }

    buffer_cursor_init(&self->cursor, 0, 0);
    return 0;
}

void buffer_deinit(struct buffer *self) {
    for (int32_t i = 0; i < self->numChunks; ++i) {
        buffer_chunk_deinit(&self->chunks[i]);
    }
    free(self->chunks);
}

void buffer_setCursor(struct buffer *self, int64_t row, int64_t col);

void buffer_moveCursor(struct buffer *self, int64_t offset) {
    self->cursor.chunkOffset += offset;
    if (self->cursor.chunkOffset < 0) {
        // Negative chunkOffset means we need to go to an earlier chunk.
        do {
            --self->cursor.chunkIndex;
            assert(buffer_cursorInValidState(self));
            int64_t chunkLength = self->chunks[self->cursor.chunkIndex].length;
            self->cursor.chunkOffset += chunkLength;
        } while (self->cursor.chunkOffset < 0);
    } else {
        int64_t chunkLength = self->chunks[self->cursor.chunkIndex].length;
        // chunkOffset larger than the chunk length means we need to go to a later chunk.
        while (self->cursor.chunkOffset >= chunkLength) {
            ++self->cursor.chunkIndex;
            self->cursor.chunkOffset -= chunkLength;
            assert(buffer_cursorInValidState(self));
            chunkLength = self->chunks[self->cursor.chunkIndex].length;
        }
    }
}

void buffer_insertBeforeCursor(struct buffer *self, char c) {

}

void buffer_deleteAtCursor(struct buffer *self) {
    assert(
        self->cursor.chunkIndex < self->numChunks &&
        self->cursor.chunkOffset < self->chunks[self->cursor.chunkIndex].length
    );

    struct buffer_chunk *chunk = &self->chunks[self->cursor.chunkIndex];
    buffer_chunk_deleteAt(chunk, self->cursor.chunkOffset);

    if (chunk->length == 0) {
        buffer_deleteChunk(self, self->cursor.chunkIndex);
        
        // Don't have to do anything with cursor since we must've been at offset 0
        // already and the chunk gets replaced by the next one.
        assert(self->cursor.chunkOffset == 0);
    } else if (self->cursor.chunkOffset >= chunk->length) {
        // Cursor is at an invalid chunkOffset because we deleted the right most character in the chunk.
        // Move to offset 0 of the next chunk.
        ++self->cursor.chunkIndex;
        self->cursor.chunkOffset = 0;
    }
}