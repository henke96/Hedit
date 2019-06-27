#pragma once
#include <inttypes.h>
#include <assert.h>

#define BUFFER_MEMORY_ALLOCATION_ERROR -1

struct buffer_modification {
    int64_t intervalStart;
    int64_t intervalEnd;
    int64_t insertLength;
    char *insert;
};

struct buffer_cursor {
    int64_t offset; // Offset from previous modification.
    int32_t prevModificationIndex; // -1 if none.
    char currentChar;
};

struct buffer {
    struct buffer_cursor cursor;
    const char *text;
    int64_t textLength;
    struct buffer_modification *modifications;
    int32_t numModifications;
};

void buffer_init(struct buffer *self, const char *text, int64_t textLength);
void buffer_deinit(struct buffer *self);

void buffer_moveCursor(struct buffer *self, int64_t offset);
void buffer_insertAtCursor(struct buffer *self, char c);
void buffer_deleteAtCursor(struct buffer *self);

static inline char buffer_getAtCursor(const struct buffer *self) {
    assert(
        self->cursor.prevModificationIndex != -1 || (
            // Index is -1.
            self->cursor.offset < self->textLength
        )
    );
    assert(
        (
            self->cursor.prevModificationIndex == -1 ||
            self->cursor.prevModificationIndex != self->numModifications - 1
        ) || (
            // It's the last modification.
            (
                self->modifications[self->numModifications - 1].intervalEnd +
                self->cursor.offset - self->modifications[self->numModifications - 1].insertLength
            ) < self->textLength
        )
    );
    return self->cursor.currentChar;
    /*if (self->cursor.prevModificationIndex < 0) {
        assert(self->cursor.prevModificationIndex == -1);
        return self->text[self->cursor.offset];
    }

    struct buffer_modification *prevModification = &self->modifications[self->cursor.prevModificationIndex];
    if (self->cursor.offset < prevModification->insertLength) {
        return prevModification->insert[self->cursor.offset];
    }

    return self->text[
        prevModification->intervalEnd + self->cursor.offset - prevModification->insertLength
    ];*/
}