#include "main/ui.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// Returns line length if successful.
static int64_t ui_printLine(const struct ui *self) {
    struct buffer_cursor cursorCopy;
    buffer_cursor_INIT_COPY(cursorCopy, self->lineCursor);

    if (printf("%" PRId64 ":", self->lineCursor.bufferOffset) < 0) return -1;

    while (1) {
        struct bufferChunk chunk = buffer_getCursorChunk(self->buffer, &cursorCopy);
        assert(chunk.length > 0);

        for (int64_t i = chunk.cursorOffset; i < chunk.length; ++i) {
            if (chunk.text[i] != '\n') continue;

            buffer_moveCursor(self->buffer, &cursorCopy, (i - chunk.cursorOffset) + 1); // Move 1 past the linefeed.
            if (printf("%.*s", (int)(i - chunk.cursorOffset), &chunk.text[chunk.cursorOffset]) < 0) return -1;
            goto foundLineEnd;
        }
        // Line end not in chunk.
        if (printf("%.*s", (int)(chunk.length - chunk.cursorOffset), &chunk.text[chunk.cursorOffset]) < 0) return -1;
        buffer_moveCursor(self->buffer, &cursorCopy, (chunk.length - chunk.cursorOffset));
        if (cursorCopy.bufferOffset == self->buffer->length) break;
    }
    foundLineEnd:
    return cursorCopy.bufferOffset - self->lineCursor.bufferOffset;
}

static int ui_parseInt64(const char *input, int64_t inputLength, int64_t *out_result) {
    if (inputLength < 1) return -1;

    uint64_t cutoff;
    int cutlim;
    int negative;
    if (input[0] == '-') {
        if (inputLength < 2) return -2;
        negative = 1;
        cutoff = -((uint64_t)INT64_MIN) / 10;
        cutlim = -((uint64_t)INT64_MIN) % 10;
    } else {
        negative = 0;
        cutoff = INT64_MAX / 10;
        cutlim = INT64_MAX % 10;
    }

    uint64_t result = 0;
    int ch;
    for (int64_t i = negative; i < inputLength; ++i) {
        ch = input[i];
        if (ch >= '0' && ch <= '9') {
            ch -= '0';
        } else return -3;

        if (result > cutoff || (result == cutoff && ch > cutlim)) {
            if (negative) *out_result = INT64_MIN;
            else *out_result = INT64_MAX;
            return 1;
        }
        result = 10 * result + ch;
    }
    if (negative) result = -result;
    *out_result = (int64_t)result;
    return 0;
}

// Returns length if successful.
static int64_t ui_readInput(struct ui *self, bool stripLineEnd) {
    int64_t inputBufferIndex = 0;

    while (1) {
        int ch = getchar();
        if (ch == EOF) return -1;
        // Realloc buffer if needed.
        if (inputBufferIndex == self->inputBufferCapacity) {
            if (inputBufferIndex == INT64_MAX) return -2;
            int64_t newCapacity = inputBufferIndex + 1;

            void *newInputBuffer = realloc(self->inputBuffer, newCapacity);
            if (!newInputBuffer) return -3;

            self->inputBuffer = newInputBuffer;
            self->inputBufferCapacity = newCapacity;
        }
        self->inputBuffer[inputBufferIndex++] = ch;
        if (ch == '\n') break;
    }
    if (stripLineEnd) {
        if (inputBufferIndex > 1 && self->inputBuffer[inputBufferIndex - 2] == '\r') {
            return inputBufferIndex - 2;
        }
        return inputBufferIndex - 1;
    }
    return inputBufferIndex;
}

static bool ui_checkCommand(struct ui *self, int64_t *inout_inputLength, const char *command, int commandLength) {
    if (*inout_inputLength >= commandLength && memcmp(self->inputBuffer, command, commandLength) == 0) {
        *inout_inputLength -= commandLength;
        memmove(self->inputBuffer, self->inputBuffer + commandLength, *inout_inputLength);
        return true;
    }
    return false;
}

static void ui_init(struct ui *self, struct buffer *buffer) {
    self->buffer = buffer;
    self->inputBuffer = NULL;
    self->inputBufferCapacity = 0;

    buffer_cursor_init(&self->lineCursor, self->buffer);
    buffer_registerCursor(self->buffer, &self->lineCursor);
}

static int ui_run(struct ui *self) {
    int64_t lineLength = ui_printLine(self);
    if (lineLength < 0) return -1;

    while (1) {
        int64_t inputLength = ui_readInput(self, true);
        if (inputLength < 0) return -2;

        //printf("%.*s\n", (int)inputLength, self->inputBuffer);

        if (inputLength == 0) {
            buffer_moveCursor(self->buffer, &self->lineCursor, lineLength);
        } else if (ui_checkCommand(self, &inputLength, "edit", 4)) {
            if (inputLength == 0) {
                inputLength = ui_readInput(self, false);
                if (inputLength < 0) goto cancel; 
            }
            // TODO: Better error handling, might need changes in buffer..
            if (buffer_deleteAtCursor(self->buffer, &self->lineCursor, lineLength) < 0) return -3;
            if (buffer_insertAtCursor(self->buffer, &self->lineCursor, self->inputBuffer, inputLength) < 0) return -4;
        } else if (ui_checkCommand(self, &inputLength, "goto", 4)) {
            if (inputLength == 0) {
                inputLength = ui_readInput(self, true);
                if (inputLength < 0) goto cancel;
            }
            int64_t gotoLocation;
            if (ui_parseInt64(self->inputBuffer, inputLength, &gotoLocation) >= 0) {
                if (gotoLocation < 0) {
                    gotoLocation += (1 + self->buffer->length); // -1 puts you after the last character.
                    if (gotoLocation < 0) gotoLocation = 0;
                } else if (gotoLocation > self->buffer->length) {
                    gotoLocation = self->buffer->length;
                }
                if (buffer_MOVE_CURSOR_TO(*self->buffer, self->lineCursor, gotoLocation) < 0) return -5;
                // TODO: Move back to previous line start.
            } else {
                printf("Invalid integer!!\n");
            }
        }
        cancel:
        lineLength = ui_printLine(self);
        if (lineLength < 0) return -6;
    }
}