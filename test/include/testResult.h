#pragma once
#include <stdlib.h>
#include <stdbool.h>

struct testResult {
    const char *name;
    bool success;
    const char *message;
    const char *file;
    int line;
};

static inline void testResult_init_success(struct testResult *self, const char *name) {
    self->name = name;
    self->success = true;
}

static inline void testResult_init_fail(struct testResult *self, const char *name, const char *file, int line, const char *message) {
    self->name = name;
    self->success = false;
    self->file = file;
    self->line = line;
    self->message = message;
}

static inline bool *testResult_wasSuccessful(const struct testResult *self) {
    return self->success;
}

static inline void testResult_printResult(const struct testResult *self) {
    if (testResult_wasSuccessful(self)) {
        printf("Test '%s' passed.\n", self->name);
    } else {
        printf("Test '%s' FAILED!\n\tFile: %s\n\tLine: %d\n\tMessage: %s\n", self->name, self->file, self->line, self->message);
    }
}