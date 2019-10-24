#include "main/textBufferView.h"
#include "main/buffer.h"
#include <stdlib.h>
#include <string.h>

#define SELF ((struct textBufferView *)self)

static inline void textBufferView_cursor_init(struct textBufferView_cursor *self, struct textBufferView *buffer) {
    buffer_cursor_init(&self->cursor, buffer->bufferView.buffer);
    self->column = 0;
    self->line = 0;
}

static inline void textBufferView_cursor_deinit(struct textBufferView_cursor *self) {
    buffer_cursor_deinit(&self->cursor);
}

void textBufferView_init(struct textBufferView *self, const char *text, int64_t textLength) {
    self->bufferView.type = bufferView_type_TEXT;
    self->cursors = NULL;
    self->numCursors = 0;
}

int textBufferView_createCursor(struct bufferView *self, int32_t *out_cursor) {
    SELF->cursors = realloc(SELF->cursors, sizeof(SELF->cursors[0])*(SELF->numCursors + 1));
    if (SELF->cursors == NULL) return -1;
    textBufferView_cursor_init(&SELF->cursors[SELF->numCursors], self);
    *out_cursor = SELF->numCursors;
    ++SELF->numCursors;
    return 0;
}

void textBufferView_destroyCursor(struct bufferView *self, int32_t cursor) {
    textBufferView_cursor_deinit(&SELF->cursors[cursor], self);
    --SELF->numCursors;
    memcpy(&SELF->cursors[cursor], &SELF->cursors[cursor + 1], sizeof(SELF->cursors[0])*(SELF->numCursors - cursor));
    struct textBufferView_cursor *newCursors = realloc(SELF->cursors, sizeof(SELF->cursors[0])*SELF->numCursors);
    if (newCursors != NULL) {
        SELF->cursors = newCursors;
    }
}