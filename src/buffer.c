#include "buffer.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static int buffer_modification_init(
    struct buffer_modification *self,
    int64_t intervalStart,
    int64_t intervalLength,
    int64_t insertLength,
    char *insertStart
) {
    char *insert = malloc(insertLength);
    if (!insert) return -1;

    self->intervalStart = intervalStart;
    self->intervalEnd = intervalStart + intervalLength;
    self->insertEnd = insertStart + insertLength;
    // Above might be UB when insertEnd is NULL, even though insertLength is 0?
    assert(insertStart != NULL || self->insertEnd == NULL);

    self->insertLength = insertLength;
    return 0;
}

static void buffer_modification_deinit(struct buffer_modification *self) {
    char *insertStart = self->insertEnd - self->insertLength;
    // Above might be UB when insertEnd is NULL, even though insertLength is 0?
    assert(self->insertEnd != NULL || insertStart == NULL);
    //free(insertStart);
}

static void testInit(struct buffer *self) {
    self->numModifications = 1;
    self->modifications = malloc(sizeof(self->modifications[0])*self->numModifications);
    buffer_modification_init(&self->modifications[0], 5, 1, 2, " henrik ");
}

void buffer_init(struct buffer *self, const char *text, int64_t textLength) {
    assert(textLength >= 0);
    self->text = text;
    self->textLength = textLength;
    
    self->numModifications = 0;
    self->modifications = NULL;

    // Initialize cursor.
    self->cursor.offset = 0;
    self->cursor.prevModificationIndex = -1;
    testInit(self);
}

void buffer_deinit(struct buffer *self) {
    for (int32_t i = 0; i < self->numModifications; ++i) {
        buffer_modification_deinit(&self->modifications[i]);
    }
    free(self->modifications);
}

void buffer_setCursor(struct buffer *self, int64_t row, int64_t col);

void buffer_moveCursor(struct buffer *self, int64_t offset) {
    int64_t cursorOffset = self->cursor.offset + offset;
    if (offset >= 0) {
        if (self->cursor.offset < 0) {
            if (cursorOffset < 0) {
                self->cursor.offset = cursorOffset;
                return;
            }
            cursorOffset += self->modifications[self->cursor.prevModificationIndex].intervalEnd;
        }

        int32_t next = self->cursor.prevModificationIndex + 1;
        for (; next < self->numModifications;) {
            assert(cursorOffset >= 0);
            if (cursorOffset < self->modifications[next].intervalStart) {
                break;
            }
            cursorOffset -= self->modifications[next].intervalStart + self->modifications[next].insertLength;
            if (cursorOffset < 0) {
                ++next;
                break;
            }
            cursorOffset += self->modifications[next].intervalEnd;
        }
        self->cursor.prevModificationIndex = next - 1;
    } else {
        int32_t prev = self->cursor.prevModificationIndex;
        if (self->cursor.offset < 0) {
            int64_t offsetFromStart = cursorOffset + self->modifications[prev].insertLength;
            if (offsetFromStart >= 0) {
                self->cursor.offset = cursorOffset;
                return;
            }
            cursorOffset = offsetFromStart + self->modifications[prev].intervalStart;
            --prev;
        }

        for (; prev >= 0; --prev) {
            assert(cursorOffset >= 0);
            if (cursorOffset >= self->modifications[prev].intervalEnd) {
                break;
            }
            cursorOffset -= self->modifications[prev].intervalEnd;
            int64_t offsetFromStart = cursorOffset + self->modifications[prev].insertLength;
            if (offsetFromStart >= 0) {
                break;
            }
            cursorOffset = offsetFromStart + self->modifications[prev].intervalStart;
        }
        self->cursor.prevModificationIndex = prev;
    }
    self->cursor.offset = cursorOffset;
}

void buffer_insertAtCursor(struct buffer *self, char c) {

}

void buffer_deleteAtCursor(struct buffer *self) {

}