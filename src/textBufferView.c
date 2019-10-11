#include "textBufferView.h"
#include "buffer.h"
#include <stdlib.h>

#define SELF ((struct textBufferView *)self)

static inline void textBufferView_cursor_init(struct textBufferView_cursor *self, struct textBufferView *buffer) {
    buffer_cursor_init(&self->cursor, buffer->bufferView.buffer);
    self->column = 0;
    self->line = 0;
}

void textBufferView_init(struct textBufferView *self, const char *text, int64_t textLength) {
    self->bufferView.type = bufferView_type_TEXT;
    self->cursors = NULL;
    self->numCursors = 0;
}

void textBufferView_deinit(struct textBufferView *self) {

}

int textBufferView_createCursor(struct bufferView *self, int32_t *out_cursor) {
    SELF->cursors = realloc(SELF->cursors, sizeof(struct textBufferView_cursor)*(SELF->numCursors + 1));
    if (SELF->cursors == NULL) return -1;
    textBufferView_cursor_init(&SELF->cursors[SELF->numCursors], self);
    *out_cursor = SELF->numCursors;
    ++SELF->numCursors;
    return 0;
}

void textBufferView_destroyCursor(struct bufferView *self, int32_t cursor) {
    
}