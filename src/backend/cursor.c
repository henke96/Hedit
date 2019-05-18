#include "backend/cursor.h"
#include "backend/buffer.h"

void cursor_init(struct cursor *self, int32_t col, int32_t row) {
    self->col = col;
    self->row = row;
}