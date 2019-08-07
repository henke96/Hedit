#include "buffer_test.h"
#include "buffer.h"

struct testResult buffer_test_simple(void) {
    struct testResult result;

    testResult_initFail(&result, "buffer_test_simple", __FILE__, __LINE__, "uh oh");
    return result;
}
