#include "buffer.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

void buffer_init(struct buffer *self, const char *text, int64_t textLength) {
    assert(textLength >= 0);
    self->text = text;
    self->textLength = textLength;
    
    self->numModifications = 0;
    self->modifications = NULL;

    // Initialize cursor.
    self->cursor.offset = 0;
    self->cursor.prevModificationIndex = -1;
    if (self->textLength > 0) {
        self->cursor.currentChar = self->text[self->cursor.offset];
    }
}

void buffer_deinit(struct buffer *self) {
    free(self->modifications);
}

void buffer_setCursor(struct buffer *self, int64_t row, int64_t col);

void buffer_moveCursor(struct buffer *self, int64_t offset) {
    int32_t i = self->cursor.prevModificationIndex;
    int64_t cursorOffset = self->cursor.offset + offset;
    if (offset >= 0) {
        if (i < 0) {
            assert(i == -1);
            if (self->numModifications == 0 || cursorOffset < self->modifications[0].intervalStart) {
                assert(cursorOffset >= 0 && cursorOffset <= self->textLength);

                self->cursor.offset = cursorOffset;
                self->cursor.currentChar = self->text[cursorOffset];
                return;
            }
            i = 0;
            cursorOffset -= self->modifications[0].intervalStart;
        }
        assert(self->numModifications > 0);

        while (1) {
            if (cursorOffset < self->modifications[i].insertLength) {
                // We are inside inserted text.
                self->cursor.currentChar = self->modifications[i].insert[cursorOffset];
                goto skip_setting_currentChar;
            }
            // We are beyond the modification.
            
            if (i == self->numModifications - 1) {
                // We are after the last modification.
                assert(self->modifications[i].intervalStart + cursorOffset <= self->textLength);
                break;
            }
            assert(i < self->numModifications - 1);

            int64_t newTextOffset = (
                self->modifications[i].intervalEnd +
                cursorOffset - self->modifications[i].insertLength // Cursor offset beyond the end.
            );

            if (newTextOffset >= self->modifications[i + 1].intervalStart) {
                // We are past the next modification.
                ++i;
                cursorOffset = newTextOffset - self->modifications[i].intervalStart;
            } else {
                // We are between prev and next.
                break;
            }
        }
    } else {
        // Move backwards.

        if (i < 0) {
            assert(i == -1);
            assert(cursorOffset >= 0 && cursorOffset <= self->textLength);
            assert(
                self->numModifications == 0 ||
                cursorOffset < self->modifications[0].intervalStart
            );

            self->cursor.offset = cursorOffset;
            self->cursor.currentChar = self->text[cursorOffset];
            return;
        }
        assert(self->numModifications > 0);

        while (1) {
            if (cursorOffset >= 0) {
                // We are past the modification.
                if (cursorOffset < self->modifications[i].insertLength) {
                    // We are inside inserted text.
                    self->cursor.currentChar = self->modifications[i].insert[cursorOffset];
                    goto skip_setting_currentChar;
                }
                // We are beyond it.
                break;
            }
            // We are before the modification.
            
            // Make it be an offset from 0 instead of from the previous modification start.
            cursorOffset += self->modifications[i].intervalStart;
            
            --i;
            if (i < 0) {
                // We are before the first modification.
                assert(cursorOffset >= 0);
                self->cursor.currentChar = self->text[cursorOffset];
                goto skip_setting_currentChar;
            }

            // Adjust the offset so it's relative to the new previous modification.
            cursorOffset -= self->modifications[i].intervalEnd;
            cursorOffset += self->modifications[i].insertLength;
        }
    }
    self->cursor.currentChar = self->text[self->modifications[i].intervalEnd + cursorOffset - self->modifications[i].insertLength];
    skip_setting_currentChar:
    self->cursor.offset = cursorOffset;
    self->cursor.prevModificationIndex = i;
}

void buffer_insertAtCursor(struct buffer *self, char c) {

}

void buffer_deleteAtCursor(struct buffer *self) {

}