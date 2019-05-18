#pragma once
#include <inttypes.h>
#include <assert.h>

struct cursor {
    int32_t row, col;
};

void cursor_init(struct cursor *self, int32_t col, int32_t row);
static inline void cursor_deinit(struct cursor *self) {}

static inline void cursor_move(struct cursor *self, int32_t cols, int32_t rows) {
    self->col += cols;
    self->row += rows;
    assert(
        self->col >= 0 &&
        self->row >= 0
    );
}

static inline void cursor_moveTo(struct cursor *self, int32_t col, int32_t row) {
    self->col = col;
    self->row = row;
    assert(
        self->col >= 0 &&
        self->row >= 0
    );
}

static inline int32_t cursor_col(const struct cursor *self) {
    return self->col;
}

static inline int32_t cursor_row(const struct cursor *self) {
    return self->row;
}
