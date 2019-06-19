#include "buffer.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static char getCharAt(struct buffer *self, int64_t bufferOffset) {
    assert(bufferOffset >= 0);

    int64_t offset = bufferOffset; // An offset into text that will be calculated with modifications in mind.
    for (int32_t i = 0; i < self->numModifications; ++i) {
        struct buffer_modification *nextModification = &self->modifications[i];
        
        if (offset >= nextModification->textOffset) {
            // This modification affects us since it's earier than the current offset.

            if (offset < nextModification->textOffset + nextModification->delta) {
                // The character is inside the modification, and it is an insertion.
                assert(nextModification->insert && nextModification->delta > 0);

                int64_t insertTextOffset = offset - nextModification->textOffset;
                return nextModification->insert[insertTextOffset];
            } else {
                // We are beyond this modification, adjust for it.
                offset -= nextModification->delta;
            }
        } else {
            // We're out of modifications that are before offset.
            break;
        }
    }
    return self->text[offset];
}

static inline void buffer_cursor_init(
    struct buffer_cursor *self
) {
    self->textOffset = 0;
}

int buffer_init(struct buffer *self, const char *text, int64_t textLength) {
    assert(textLength >= 0);
    self->text = text;
    self->textLength = textLength;
    
    self->numModifications = 0;
    self->modifications = NULL;
    return 0;
}

void buffer_deinit(struct buffer *self) {
    free(self->modifications);
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