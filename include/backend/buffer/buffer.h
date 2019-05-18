#pragma once
#include "backend/row.h"


struct buffer {
    struct textChunk *textChunks;
    struct fenwickTreeI32 *textChunkRows;
    int32_t numTextChunks;
};

void buffer_init(struct buffer *self);
void buffer_deinit(struct buffer *self);

int32_t buffer_numCols(const struct buffer *self, int32_t row);
int32_t buffer_numRows(const struct buffer *self);

int buffer_addBefore(struct buffer *self, int32_t col, int32_t row, char c);
void buffer_removeBefore(struct buffer *self, int32_t col, int32_t row, char c);
void buffer_set(struct buffer *self, int32_t col, int32_t row, char c);

struct row buffer_getRow(const struct buffer *self, int32_t row);