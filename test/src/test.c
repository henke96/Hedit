#if defined(NDEBUG)
#error NDEBUG defined when building tests!
#endif

#include "test.h"
#include "buffer_test.h"

#include <stdio.h>
#include <inttypes.h>

static test_function_t *tests[] = {
    buffer_test_simple,
};

#define NUM_TESTS (int)(sizeof(tests)/sizeof(tests[0]))

void printTestFail(const char *name, const char *file, int line) {
    printf("Test '%s' FAILED! (%s:%d)\n", name, file, line);
}

int main(int argc, char **argv) {
    int32_t numFailed = 0;

    const char *numTestString = "tests";
    if (NUM_TESTS == 1) {
        numTestString = "test";
    } else if (NUM_TESTS <= 0) {
        printf("No tests to run!\n");
        return 0;
    }

    printf("Running %d %s...\n\n", NUM_TESTS, numTestString);

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
        printf("All tests passed! (%d %s)\n", NUM_TESTS, numTestString);
    } else {
        const char *numFailedString = "tests";
        if (numFailed == 1) {
            numFailedString = "test";
        }
        printf("%d %s failed! (Out of %d %s)\n", numFailed, numFailedString, NUM_TESTS, numTestString);
    }
    return 0;
}
