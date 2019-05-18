#pragma once
#include <inttypes.h>

struct row {
    int32_t length;
};

void row_init(struct row *self);
static inline void row_deinit(struct row *self) {}

static inline int32_t row_length(const struct row *self) {
    return self->length;
}

char row_get(const struct row *self, int32_t pos);
