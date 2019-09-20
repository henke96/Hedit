#include "textBuffer.h"

void textBuffer_init(struct textBuffer *self, const char *text, int64_t textLength, bool trackLines) {
    buffer_init(&self->buffer, text, textLength);
    self->trackingLines = trackLines;
}

int textBuffer_registerCursor(struct textBuffer *self, struct textBuffer_cursor *cursor) {
   if (buffer_registerCursor(&self->buffer, &cursor->cursor) < 0) {
       return -1;
   }//TODO
}

void textBuffer_unregisterCursor(struct textBuffer *self, struct textBuffer_cursor *cursor) {

}

void textBuffer_deinit(struct textBuffer *self) {
    buffer_deinit(&self->buffer);
}

void textBuffer_cursor_init(struct textBuffer_cursor *self, const struct textBuffer *buffer) {
    buffer_cursor_init(&self->cursor, &buffer->buffer);
    self->currentLine = 0;
}