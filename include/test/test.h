#pragma once
#include <stdbool.h>

struct test_result {
    const char *testName;
    bool success;
};

static inline struct test_result test_result_create(bool success, const char *testName) {
    return (struct test_result) {
        .testName = testName,
        .success = success
    };
}

typedef struct test_result test_function_t(void);

void printTestFail(const char *testName, const char *file, int line);
