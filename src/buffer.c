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
    const char *insertStart
) {
    assert(intervalLength >= 0);
    assert(insertLength >= 0);
    if (insertLength > 0) {
        char *insert = malloc(insertLength);
        if (!insert) return -1;

        memcpy(insert, insertStart, insertLength);
        self->insertEnd = insert + insertLength;
    }

    self->intervalStart = intervalStart;
    self->intervalEnd = intervalStart + intervalLength;

    self->insertLength = insertLength;
    return 0;
}

static int insertIntoModification(struct buffer *self, int32_t index, int64_t insertOffset, const char *str, int64_t strLength) {
    assert(strLength >= 0);
    assert(index >= 0 && index < self->numModifications);

    struct buffer_modification *modification = &self->modifications[index];
    assert(insertOffset >= 0 && insertOffset <= modification->insertLength);

    /* TODO: Needs to perform cursor corrections.
    int64_t shortestLength = modification->intervalEnd - modification->intervalStart;
    if (strLength < shortestLength) shortestLength = strLength;
    
    // Try to optimize if insertion at edges.
    if (insertOffset == 0) {
        int64_t i = 0;
        for (; i < shortestLength; ++i) {
            if (str[i] != self->text[modification->intervalStart + i]) break;
        }
        if (i) {
            modification->intervalStart += i;
            str += i;
            strLength -= i;
            if (strLength == 0) return 0;
        }
    } else if (insertOffset == modification->insertLength) {
        int64_t i = 1;
        for (; i <= shortestLength; ++i) {
            if (str[strLength - i] != self->text[modification->intervalEnd - i]) break;
        }
        if (--i) {
            modification->intervalEnd -= i;
            strLength -= i;
            if (strLength == 0) return 0;
        }
    }
    */

    if (modification->insertLength > 0) {
        int64_t newLength = modification->insertLength + strLength;
        char *insert = realloc(modification->insertEnd - modification->insertLength, newLength);
        if (!insert) return -1;

        memmove(insert + insertOffset + strLength, insert + insertOffset, modification->insertLength - insertOffset);
        memcpy(insert + insertOffset, str, strLength);
        modification->insertEnd = insert + newLength;
        modification->insertLength = newLength;
    } else {
        assert(modification->insertLength == 0);

        char *insert = malloc(strLength);
        if (!insert) return -1;

        memcpy(insert, str, strLength);
        modification->insertEnd = insert + strLength;
        modification->insertLength = strLength;
    }
    return 0;
}

static void buffer_modification_deinit(struct buffer_modification *self) {
    if (self->insertLength > 0) {
        char *insertStart = self->insertEnd - self->insertLength;
        free(insertStart);
    }
}

void buffer_init(struct buffer *self, const char *text, int64_t textLength) {
    assert(textLength >= 0);
    self->text = text;
    self->textLength = textLength;
    self->bufferLength = textLength;
    self->numModifications = 0;
    self->modifications = NULL;

    // Initialize cursor.
    self->cursor.offset = 0;
    self->cursor.prevModificationIndex = -1;
    self->cursor.bufferOffset = 0;
}

void buffer_deinit(struct buffer *self) {
    for (int32_t i = 0; i < self->numModifications; ++i) {
        buffer_modification_deinit(&self->modifications[i]);
    }
    free(self->modifications);
}

void buffer_moveCursor(struct buffer *self, int64_t offset) {
    self->cursor.bufferOffset += offset;
    assert(self->cursor.bufferOffset >= 0 && self->cursor.bufferOffset <= self->bufferLength);

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
        for (; next < self->numModifications; ++next) {
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

static int insertModificationAt(struct buffer *self, int32_t index) {
    struct buffer_modification *modifications = realloc(self->modifications, sizeof(modifications[0]) * (self->numModifications + 1));
    if (!modifications) return -1;
    
    memmove(modifications + index + 1, modifications + index, self->numModifications - index);

    self->modifications = modifications;
    ++self->numModifications;
    return 0;
}

static void deleteModificationAt(struct buffer *self, int32_t index) {
    struct buffer_modification *modifications = realloc(self->modifications, sizeof(modifications[0]) * (self->numModifications - 1));
    if (modifications) {
        self->modifications = modifications;
    }
    
    memmove(self->modifications + index, self->modifications + (index + 1), self->numModifications - (index + 1));

    --self->numModifications;
}

int buffer_insertAtCursor(struct buffer *self, const char *str, int64_t strLength) {
    assert(strLength > 0);
    if (self->cursor.offset < 0) {
        struct buffer_modification *prevModification = &self->modifications[self->cursor.prevModificationIndex];
        if (insertIntoModification(self, self->cursor.prevModificationIndex, prevModification->insertLength + self->cursor.offset, str, strLength) < 0) {
            return BUFFER_MEMORY_ALLOCATION_ERROR;
        }
        self->cursor.offset -= strLength;
    } else {
        if (self->cursor.prevModificationIndex >= 0) {
            struct buffer_modification *prevModification = &self->modifications[self->cursor.prevModificationIndex];
            if (self->cursor.offset == prevModification->intervalEnd) {
                if (insertIntoModification(self, self->cursor.prevModificationIndex, prevModification->insertLength, str, strLength) < 0) {
                    return BUFFER_MEMORY_ALLOCATION_ERROR;
                }
                self->cursor.offset = -strLength;
                goto success;
            }
        }

        int32_t newModificationIndex = self->cursor.prevModificationIndex + 1;
        if (insertModificationAt(self, newModificationIndex) < 0) {
            return BUFFER_MEMORY_ALLOCATION_ERROR;
        }

        if (buffer_modification_init(&self->modifications[newModificationIndex], self->cursor.offset, 0, strLength, str) < 0) {
            deleteModificationAt(self, newModificationIndex);
            return BUFFER_MEMORY_ALLOCATION_ERROR;
        }
        self->cursor.offset = -strLength;
        self->cursor.prevModificationIndex = newModificationIndex;
    }
    success:
    self->bufferLength += strLength;
    return 0;
}

void buffer_deleteAtCursor(struct buffer *self, int64_t length) {
    if (self->cursor.offset < 0) {

    } else {

    }
}