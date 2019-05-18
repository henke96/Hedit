#if defined(NDEBUG)
#error NDEBUG defined when building tests!
#endif

#include "fenwickTree/test_fenwickTreeI32.h"
#include <stdio.h>

int main(int argc, char **argv) {
    test_fenwicktreeI32();

    printf("All tests passed!\n");
}