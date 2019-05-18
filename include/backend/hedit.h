#pragma once
#include "buffer.h"
#include "cursor.h"
#include "row.h"
#include <inttypes.h>

struct hedit {
    struct buffer buffer;
    struct cursor cursor;
};

void hedit_init(struct hedit *self);
void hedit_deinit(struct hedit *self);

// Loads content from file at path. If path is NULL, content will become empty.
int hedit_load(struct hedit *self, const char *path);
// Saves content to file at path. path can't be NULL.
int hedit_save(struct hedit *self, const char *path);

static inline void hedit_moveCursor(struct hedit *self, int32_t cols, int32_t rows) {
    cursor_move(&self->cursor, rows, rows);
    assert(
        cursor_row(&self->cursor) < buffer_numRows(&self->buffer) &&
        cursor_col(&self->cursor) < buffer_numCols(&self->buffer, cursor_row(&self->cursor))
    );
}

static inline void hedit_moveCursorTo(struct hedit *self, int32_t col, int32_t row) {
    cursor_moveTo(&self->cursor, row, row);
    assert(
        cursor_row(&self->cursor) < buffer_numRows(&self->buffer) &&
        cursor_col(&self->cursor) < buffer_numCols(&self->buffer, cursor_row(&self->cursor))
    );
}

static inline int32_t hedit_numRows(struct hedit *self) {
    return buffer_numRows(self->buffer);
}

static inline int32_t hedit_numCols(struct hedit *self, int32_t col) {
    return buffer_numCols(self->buffer, col);
}

static inline int hedit_addBefore(struct hedit *self, char c) {
    return buffer_addBefore(self->buffer, cursor_col(&self->cursor), cursor_row(&self->cursor), c);
}

static inline void hedit_removeBefore(struct hedit *self) {
    buffer_removeBefore(self->buffer, cursor_col(&self->cursor), cursor_row(&self->cursor));
}

static inline struct row hedit_getRow(const struct hedit *self, int32_t row) {
    return buffer_getRow(self->buffer, cursor_row(&self->cursor));
}