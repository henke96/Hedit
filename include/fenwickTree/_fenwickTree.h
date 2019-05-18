#pragma once
#include <inttypes.h>
#include <string.h>

#define _fenwickTree(T, suffix) \
struct fenwickTree##suffix { \
    int32_t length; \
    T tree[]; \
}; \
\
static inline void fenwickTree##suffix##_init( \
    struct fenwickTree##suffix *self, \
    int32_t length \
) { \
    self->length = length; \
    memset(self->tree, 0, sizeof(T)*(1 + length)); \
} \
\
void fenwickTree##suffix##_add(struct fenwickTree##suffix *self, int32_t index, T value); \
T fenwickTree##suffix##_sum(struct fenwickTree##suffix *self, int32_t endIndex); \

#define _fenwickTree_SIZE(T, suffix, numElements) ( \
    sizeof(struct fenwickTree##suffix) + sizeof(T)*(1 + numElements) \
) \

#define _fenwickTree_IMPL(T, suffix) \
static inline int32_t _fenwickTree_leastSignificantBit(int32_t n) { \
    return n & -n; \
} \
\
void fenwickTree##suffix##_add(struct fenwickTree##suffix *self, int32_t index, T value) { \
    ++index; \
    do { \
        self->tree[index] += value; \
        index += _fenwickTree_leastSignificantBit(index); \
    } while (index <= self->length); \
} \
\
T fenwickTree##suffix##_sum(struct fenwickTree##suffix *self, int32_t endIndex) { \
    T result = 0; \
    while (endIndex) { \
        result += self->tree[endIndex]; \
        endIndex -= _fenwickTree_leastSignificantBit(endIndex); \
    } \
    return result; \
} \

