#if defined(NDEBUG)
#error NDEBUG defined when building tests!
#endif

#include "test.h"
#include "buffer_test.h"

#include <stdio.h>
#include <inttypes.h>

static test_function_t *tests[] = {
    buffer_test_simple
};

#define NUM_TESTS (int)(sizeof(tests)/sizeof(tests[0]))

int main(int argc, char **argv) {
    int32_t numFailed = 0;

    const char *numTestString = "tests";
    if (NUM_TESTS == 1) {
        numTestString = "test";
    }

    printf("Running %d %s!\n\n", NUM_TESTS, numTestString);

    for (int32_t i = 0; i < NUM_TESTS; ++i) {
        struct testResult result = tests[i]();
        if (!testResult_wasSuccessful(&result)) {
            ++numFailed;
        }
        testResult_printResult(&result);
    }

    printf("\n");

    if (numFailed == 0) {
        printf("All %d %s passed!\n", NUM_TESTS, numTestString);
    } else {
        const char *numFailedString = "tests";
        if (numFailed == 1) {
            numFailedString = "test";
        }
        printf("%d %s failed! (Out of %d %s)\n", numFailed, numFailedString, NUM_TESTS, numTestString);
    }
}
