#include "main/ui.h"
#include <stdbool.h>

// Returns characters printed if successful.
static int64_t ui_printLine(const struct ui *self) {
    if (self->lineCursor.bufferOffset == self->buffer->length) return 0;

    struct buffer_cursor cursorCopy;
    buffer_cursor_INIT_COPY(cursorCopy, self->lineCursor);

    if (cursorCopy.bufferOffset != 0) {
        // Should always stand on linefeed, unless at start.
        assert(buffer_getAtCursor(self->buffer, &cursorCopy) == 10);
        buffer_moveCursor(self->buffer, &cursorCopy, 1);
    }

    while (1) {
        struct bufferChunk chunk = buffer_getCursorChunk(self->buffer, &cursorCopy);
        assert(chunk.length > 0);

        for (int64_t i = chunk.cursorOffset; i < chunk.length; ++i) {
            if (chunk.text[i] != 10) continue;

            buffer_moveCursor(self->buffer, &cursorCopy, i - chunk.cursorOffset);
            if (printf("%d:%.*s", (int)(self->lineCursor.bufferOffset), (int)(i - chunk.cursorOffset), &chunk.text[chunk.cursorOffset]) < 0) return -1;
            goto foundLineEnd;
        }
        // Line end not in chunk.
        if (printf("%d:%.*s", (int)(self->lineCursor.bufferOffset), (int)(chunk.length - chunk.cursorOffset), &chunk.text[chunk.cursorOffset]) < 0) return -1;
        buffer_moveCursor(self->buffer, &cursorCopy, (chunk.length - chunk.cursorOffset));
        if (cursorCopy.bufferOffset == self->buffer->length) break;
    }
    foundLineEnd:
    return cursorCopy.bufferOffset - self->lineCursor.bufferOffset;
}

// Returns length if successful.
static int64_t ui_readInput(struct ui *self) {
    int64_t inputBufferIndex = 0;

    while (1) {
        int ch = getchar();
        if (ch < 32) {
            if (ch == EOF) return -1;
            if (ch == 10) return inputBufferIndex;
            continue;
        }
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
    }
}

static void ui_init(struct ui *self, struct buffer *buffer) {
    self->buffer = buffer;
    self->inputBuffer = NULL;
    self->inputBufferCapacity = 0;

    buffer_cursor_init(&self->lineCursor, self->buffer);
    buffer_registerCursor(self->buffer, &self->lineCursor);
}

static int ui_run(struct ui *self) {
    int64_t lineEndOffset = ui_printLine(self);
    if (lineEndOffset < 0) return -1;

    while (1) {
        int64_t inputLength = ui_readInput(self);
        if (inputLength < 0) return -2;

        //printf("%.*s\n", (int)inputLength, self->inputBuffer);

        if (inputLength == 0) {
            buffer_moveCursor(self->buffer, &self->lineCursor, lineEndOffset);
        }
        lineEndOffset = ui_printLine(self);
        if (lineEndOffset < 0) return -3;
    }
}