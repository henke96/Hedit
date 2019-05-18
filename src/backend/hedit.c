#include "backend/hedit.h"

void hedit_init(struct hedit *self) {
    cursor_init(&self->cursor, 0, 0);
    buffer_init(&self->buffer);
}