#include "test/test.h"
#include "test/test_buffer.h"

#include <stdio.h>
#include <inttypes.h>

static test_function_t *tests[] = {
    test_buffer_simple,
    test_buffer_simpleMultiCursor
};

#define NUM_TESTS (int32_t)(sizeof(tests)/sizeof(tests[0]))

void printTestFail(const char *name, const char *file, int32_t line) {
    printf("Test '%s' FAILED! (%s:%" PRId32 ")\n", name, file, line);
}

int main(void) {
    int32_t numFailed = 0;

    const char *numTestString = "tests";
    if (NUM_TESTS == 1) {
        numTestString = "test";
    } else if (NUM_TESTS <= 0) {
        printf("No tests to run!\n");
        return 0;
    }

    printf("Running %" PRId32 " %s...\n\n", NUM_TESTS, numTestString);

    for (int32_t i = 0; i < NUM_TESTS; ++i) {
        struct test_result result = tests[i]();
        if (result.success) {
            printf("Test '%s' passed.\n", result.testName);
        } else {
            // The tests print details if failed.
            ++numFailed;
        }
    }

    printf("\n");

    if (numFailed == 0) {
        printf("All tests passed! (%" PRId32 " %s)\n", NUM_TESTS, numTestString);
    } else {
        const char *numFailedString = "tests";
        if (numFailed == 1) {
            numFailedString = "test";
        }
        printf("%d %s failed! (Out of %" PRId32 " %s)\n", numFailed, numFailedString, NUM_TESTS, numTestString);
    }
    return 0;
}
