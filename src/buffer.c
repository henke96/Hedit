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

static inline char *buffer_modification_getInsertStart(const struct buffer_modification *self) {
    return self->insertEnd - self->insertLength;
}

static void buffer_modification_deinit(struct buffer_modification *self) {
    if (self->insertLength > 0) {
        free(buffer_modification_getInsertStart(self));
    }
}

static int insertModificationAt(struct buffer *self, int32_t index) {
    assert(index >= 0);
    struct buffer_modification *modifications = realloc(self->modifications, sizeof(modifications[0]) * (self->numModifications + 1));
    if (!modifications) return -1;
    
    memmove(modifications + index + 1, modifications + index, sizeof(modifications[0]) * (self->numModifications - index));

    self->modifications = modifications;
    ++self->numModifications;
    return 0;
}

static void deleteModificationAt(struct buffer *self, int32_t index) {
    if (self->numModifications > 1) {
        memmove(self->modifications + index, self->modifications + (index + 1), sizeof(self->modifications[0]) * (self->numModifications - (index + 1)));

        struct buffer_modification *modifications = realloc(self->modifications, sizeof(modifications[0]) * (self->numModifications - 1));
        if (modifications) {
            self->modifications = modifications;
        }
    } else {
        free(self->modifications);
        self->modifications = NULL;
    }
    --self->numModifications;
}

static void deleteModificationProperly(struct buffer *self, int32_t index) {
    // Correct prevModificationIndex for all cursors.
    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
        struct buffer_cursor *cursor = self->registeredCursors[i];
        if (cursor != NULL) {
            if (cursor->prevModificationIndex >= index) {
                // Past the deleted modification.
                --cursor->prevModificationIndex;
            }
        }
    }

    buffer_modification_deinit(&self->modifications[index]);
    deleteModificationAt(self, index);
}

static int insertIntoModification(struct buffer *self, int32_t index, int64_t cursorOffset, const char *str, int64_t strLength) {
    assert(strLength >= 0);
    assert(index >= 0 && index < self->numModifications);

    struct buffer_modification *modification = &self->modifications[index];

    int64_t insertOffset = cursorOffset + modification->insertLength;
    assert(insertOffset >= 0 && insertOffset <= modification->insertLength);

    // Try to optimize if insertion at edges.
    int64_t shortestLength = modification->intervalEnd - modification->intervalStart;
    if (strLength < shortestLength) shortestLength = strLength;

    if (insertOffset == 0) {
        int64_t i = 0;
        for (; i < shortestLength; ++i) {
            if (str[i] != self->text[modification->intervalStart + i]) break;
        }
        if (i != 0) {
            modification->intervalStart += i;
            str += i;
            strLength -= i;
        }
    } else if (insertOffset == modification->insertLength) {
        int64_t i = 1;
        for (; i <= shortestLength; ++i) {
            if (str[strLength - i] != self->text[modification->intervalEnd - i]) break;
        }
        if (--i != 0) {
            modification->intervalEnd -= i;
            strLength -= i;
        }
    }

    // Check if nothing left to be done.
    if (strLength == 0) {
        if (
            modification->insertLength == 0 &&
            modification->intervalStart == modification->intervalEnd
        ) {
            // The insertion optimization must've reduced the interval to be empty.
            deleteModificationProperly(self, index);
        }
        return 0;
    }

    if (modification->insertLength > 0) {
        // Insert into existing insertion.
        int64_t newLength = modification->insertLength + strLength;
        char *insert = realloc(buffer_modification_getInsertStart(modification), newLength);
        if (!insert) return -1;

        memmove(insert + insertOffset + strLength, insert + insertOffset, modification->insertLength - insertOffset);
        memcpy(insert + insertOffset, str, strLength);
        modification->insertEnd = insert + newLength;
        modification->insertLength = newLength;

        // Fix cursor offsets.
        for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
            struct buffer_cursor *cursor = self->registeredCursors[i];
            if (cursor != NULL) {
                if (
                    cursor->prevModificationIndex == index && // Same modification.
                    cursor->offset < cursorOffset // Affected by the insert.
                ) {
                    assert(cursor->offset < 0);
                    cursor->offset -= strLength;
                }
            }
        }
    } else {
        // Create new insertion. This can't break any cursor offsets.
        assert(modification->insertLength == 0);

        char *insert = malloc(strLength);
        if (!insert) return -1;

        memcpy(insert, str, strLength);
        modification->insertEnd = insert + strLength;
        modification->insertLength = strLength;
    }
    return 0;
}

static void deleteFromModification(struct buffer *self, int32_t index, int64_t cursorOffset, int64_t deleteLength) {
    assert(deleteLength > 0);
    assert(cursorOffset < 0);

    struct buffer_modification *modification = &self->modifications[index];

    int64_t newLength = modification->insertLength - deleteLength;
    assert(newLength >= 0);

    char *oldInsert = buffer_modification_getInsertStart(modification);

    if (newLength == 0) {
        // Delete everything.

        // Correct offset for all cursors.
        for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
            struct buffer_cursor *cursor = self->registeredCursors[i];
            if (cursor != NULL) {
                if (
                    cursor->prevModificationIndex == index &&
                    cursor->offset < 0
                ) {
                    // Inside it.
                    cursor->offset = modification->intervalEnd;
                    buffer_moveCursor(self, cursor, 0); // Fixes the cursor offset incase theres no gap to next modification.
                }
            }
        }

        if (modification->intervalEnd - modification->intervalStart == 0) {
            // This modification doesn't remove or insert any text. Delete it.
            deleteModificationProperly(self, index);
        } else {
            // It removes text, but doesn't insert any.
            free(oldInsert);
            modification->insertLength = 0;
        }
    } else {
        // Delete a part of the insert.

        // Correct all cursors.
        for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
            struct buffer_cursor *cursor = self->registeredCursors[i];
            if (cursor != NULL) {
                if (
                    cursor->prevModificationIndex == index &&
                    cursor->offset < 0
                ) {
                    // Inside the modification.
                    if (cursor->offset < cursorOffset) {
                        // Before the deleted interval
                        cursor->offset += deleteLength;
                    } else if (cursor->offset < cursorOffset + deleteLength) {
                        // Inside the deleted interval.
                        cursor->offset = cursorOffset + deleteLength;
                        if (cursor->offset == 0) {
                            cursor->offset = modification->intervalEnd;
                            buffer_moveCursor(self, cursor, 0); // Fixes the cursor offset incase theres no gap to next modification.
                        }
                    }
                }
            }
        }

        int64_t deleteOffset = cursorOffset + modification->insertLength;
        assert(deleteOffset >= 0 && deleteOffset + deleteLength <= modification->insertLength);

        memmove(
            oldInsert + deleteOffset,
            oldInsert + deleteOffset + deleteLength,
            modification->insertLength - deleteOffset - deleteLength
        );

        char *insert = realloc(oldInsert, newLength);
        if (!insert) insert = oldInsert;

        modification->insertEnd = insert + newLength;
        modification->insertLength = newLength;
    }
}

void buffer_init(struct buffer *self, const char *text, int64_t textLength) {
    assert(textLength >= 0);
    self->text = text;
    self->textLength = textLength;
    self->bufferLength = textLength;
    self->numModifications = 0;
    self->modifications = NULL;
    self->registeredCursorsCapacity = 0;
    self->registeredCursors = NULL;
}

void buffer_deinit(struct buffer *self) {
    for (int32_t i = 0; i < self->numModifications; ++i) {
        buffer_modification_deinit(&self->modifications[i]);
    }
    free(self->modifications);
}

int buffer_registerCursor(struct buffer *self, struct buffer_cursor *cursor) {
    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
        if (self->registeredCursors[i] == NULL) {
            self->registeredCursors[i] = cursor;
            return 0;
        }
    }
    // No free spot.
    struct buffer_cursor **newRegisteredCursors = realloc(
        self->registeredCursors,
        sizeof(self->registeredCursors[0]) * (self->registeredCursorsCapacity + 1)
    );
    if (newRegisteredCursors == NULL) return -1;

    newRegisteredCursors[self->registeredCursorsCapacity++] = cursor;
    self->registeredCursors = newRegisteredCursors;
    return 0;
}

void buffer_unregisterCursor(struct buffer *self, struct buffer_cursor *cursor) {
    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
        if (self->registeredCursors[i] == cursor) {
            self->registeredCursors[i] = NULL;
            return;
        }
    }
    assert(0);
}

struct bufferChunk buffer_getCursorChunk(const struct buffer *self, const struct buffer_cursor *cursor) {
    struct bufferChunk chunk;
    if (cursor->offset < 0) {
        struct buffer_modification *modification = &self->modifications[cursor->prevModificationIndex];
        chunk.text = &modification->insertEnd[-modification->insertLength];
        chunk.length = modification->insertLength;
        chunk.cursorOffset = modification->insertLength + cursor->offset;
    } else {
        int64_t textOffset;
        if (cursor->prevModificationIndex < 0) {
            textOffset = 0;
        } else {
            textOffset = self->modifications[cursor->prevModificationIndex].intervalEnd;
        }
        chunk.text = &self->text[textOffset];
        int32_t nextIndex = cursor->prevModificationIndex + 1;
        if (nextIndex < self->numModifications) {
            chunk.length = self->modifications[nextIndex].intervalStart - textOffset;
        } else {
            chunk.length = self->textLength - textOffset;
        }
        chunk.cursorOffset = cursor->offset - textOffset;
    }
    return chunk;
}

void buffer_moveCursor(const struct buffer *self, struct buffer_cursor *cursor, int64_t offset) {
    cursor->bufferOffset += offset;
    assert(cursor->bufferOffset >= 0 && cursor->bufferOffset <= self->bufferLength);

    int64_t cursorOffset = cursor->offset + offset;
    if (offset >= 0) {
        if (cursor->offset < 0) {
            if (cursorOffset < 0) {
                cursor->offset = cursorOffset;
                return;
            }
            cursorOffset += self->modifications[cursor->prevModificationIndex].intervalEnd;
        }

        int32_t next = cursor->prevModificationIndex + 1;
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
        cursor->prevModificationIndex = next - 1;
    } else {
        int32_t prev = cursor->prevModificationIndex;
        if (cursor->offset < 0) {
            int64_t offsetFromStart = cursorOffset + self->modifications[prev].insertLength;
            if (offsetFromStart >= 0) {
                cursor->offset = cursorOffset;
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
        cursor->prevModificationIndex = prev;
    }
    cursor->offset = cursorOffset;
}

int buffer_insertAtCursor(struct buffer *self, const struct buffer_cursor *cursor, const char *str, int64_t strLength) {
    assert(strLength > 0);
    if (cursor->offset < 0) {
        // Insert into current modification.
        if (insertIntoModification(self, cursor->prevModificationIndex, cursor->offset, str, strLength) < 0) {
            return buffer_MEMORY_ALLOCATION_ERROR;
        }
    } else if (
        cursor->prevModificationIndex >= 0 &&
        cursor->offset == self->modifications[cursor->prevModificationIndex].intervalEnd
    ) {
        // Can append to previous modification.
        if (insertIntoModification(self, cursor->prevModificationIndex, 0, str, strLength) < 0) {
            return buffer_MEMORY_ALLOCATION_ERROR;
        }
    } else {
        // Create a new modification.
        int32_t newModificationIndex = cursor->prevModificationIndex + 1;
        if (insertModificationAt(self, newModificationIndex) < 0) {
            return buffer_MEMORY_ALLOCATION_ERROR;
        }

        if (buffer_modification_init(&self->modifications[newModificationIndex], cursor->offset, 0, strLength, str) < 0) {
            deleteModificationAt(self, newModificationIndex);
            return buffer_MEMORY_ALLOCATION_ERROR;
        }

        // Correct prevModificationIndex for all cursors.
        for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
            struct buffer_cursor *cursor2 = self->registeredCursors[i];
            if (cursor2 != NULL) {
                if (cursor2->bufferOffset >= cursor->bufferOffset) {
                    // Past the inserted modification.
                    ++cursor2->prevModificationIndex;
                }
            }
        }
    }

    // Correct bufferOffset for all cursors.
    int64_t insertBufferOffset = cursor->bufferOffset;
    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
        struct buffer_cursor *cursor2 = self->registeredCursors[i];
        if (cursor2 != NULL) {
            if (cursor2->bufferOffset >= insertBufferOffset) {
                cursor2->bufferOffset += strLength;
            }
        }
    }

    self->bufferLength += strLength;
    return 0;
}

int buffer_deleteAtCursor(struct buffer *self, const struct buffer_cursor *cursor, const int64_t length) {
    int64_t deletedLength = 0;
    while (deletedLength < length) {
        int64_t remaining = length - deletedLength;
        if (cursor->offset < 0) {
            // Inside a modification.
            int64_t shortest = -cursor->offset;
            if (remaining < shortest) shortest = remaining;

            deleteFromModification(self, cursor->prevModificationIndex, cursor->offset, shortest);
            deletedLength += shortest;
        } else {
            // Outside any modification.

            // Try to merge into next.
            if (cursor->prevModificationIndex + 1 < self->numModifications) {
                struct buffer_modification *nextModification = &self->modifications[cursor->prevModificationIndex + 1];
                int64_t untilNextModification = nextModification->intervalStart - cursor->offset;
                if (untilNextModification < remaining) {
                    // Can merge into next.
                    nextModification->intervalStart -= untilNextModification;
                    deletedLength += untilNextModification;

                    // Correct offset for all cursors.
                    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
                        struct buffer_cursor *cursor2 = self->registeredCursors[i];
                        if (cursor2 != NULL) {
                            if (
                                cursor2->bufferOffset >= cursor->bufferOffset &&
                                cursor2->bufferOffset < cursor->bufferOffset + untilNextModification
                            ) {
                                // Cursor in the deleted interval.
                                ++cursor2->prevModificationIndex;
                                if (nextModification->insertLength > 0) {
                                    cursor2->offset = -nextModification->insertLength;
                                } else {
                                    cursor2->offset = nextModification->intervalEnd;
                                    buffer_moveCursor(self, cursor2, 0); // Fixes the cursor offset incase theres no gap to next modification.
                                }
                            }
                        }
                    }
                    continue;
                }
            }
            // Couldn't merge into next, so this is guaranteed to be the last iteration.
            deletedLength += length;

            if (cursor->prevModificationIndex >= 0) {
                struct buffer_modification *prevModification = &self->modifications[cursor->prevModificationIndex];
                if (cursor->offset == prevModification->intervalEnd) {
                    // Can merge into prev.
                    prevModification->intervalEnd += remaining;

                    // Correct offset for all cursors.
                    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
                        struct buffer_cursor *cursor2 = self->registeredCursors[i];
                        if (cursor2 != NULL) {
                            if (
                                cursor2->bufferOffset >= cursor->bufferOffset &&
                                cursor2->bufferOffset < cursor->bufferOffset + remaining
                            ) {
                                // Cursor in the deleted interval.
                                cursor2->offset = prevModification->intervalEnd;
                                // Since we couldn't merge into next, there has to be a gap to next modification. So the offset doesn't need fixing.
                            }
                        }
                    }
                    break;
                }
            }
            // Couldn't merge into prev either, so create new modification.
            int32_t newModificationIndex = cursor->prevModificationIndex + 1;
            if (insertModificationAt(self, newModificationIndex) < 0) {
                return buffer_MEMORY_ALLOCATION_ERROR;
            }
            if (buffer_modification_init(&self->modifications[newModificationIndex], cursor->offset, remaining, 0, NULL) < 0) {
                deleteModificationAt(self, newModificationIndex);
                return buffer_MEMORY_ALLOCATION_ERROR;
            }

            // Correct all cursors.
            for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
                struct buffer_cursor *cursor2 = self->registeredCursors[i];
                if (cursor2 != NULL) {
                    if (cursor2->bufferOffset >= cursor->bufferOffset) {
                        // Past the inserted modification.
                        ++cursor2->prevModificationIndex;
                        if (cursor2->bufferOffset < cursor->bufferOffset + remaining) {
                            // Cursor in the deleted interval.
                            cursor2->offset = self->modifications[newModificationIndex].intervalEnd;
                            // Since we couldn't merge into next, there has to be a gap to next modification. So the offset doesn't need fixing.
                        }
                    }
                }
            }
            break;
        }
    }

    // Correct bufferOffset for all cursors.
    for (int32_t i = 0; i < self->registeredCursorsCapacity; ++i) {
        struct buffer_cursor *cursor2 = self->registeredCursors[i];
        if (cursor2 != NULL) {
            if (cursor2->bufferOffset > cursor->bufferOffset) {
                // Any cursor past the delete offset gets moved.
                int64_t moveLength = cursor2->bufferOffset - cursor->bufferOffset;
                if (moveLength > deletedLength) moveLength = deletedLength;

                cursor2->bufferOffset -= moveLength;
            }
        }
    }

    self->bufferLength -= length;
    return 0;
}

void buffer_cursor_init(struct buffer_cursor *self, const struct buffer *buffer) {
    self->bufferOffset = 0;
    self->offset = 0;
    self->prevModificationIndex = -1;
    buffer_moveCursor(buffer, self, 0); // Fix offset incase there's a modification at the start.
}