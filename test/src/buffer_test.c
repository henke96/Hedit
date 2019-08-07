#include "buffer_test.h"
#include "buffer.h"

struct testResult buffer_test_simple(void) {
    struct testResult result;

    testResult_init_fail(&result, "buffer_test_simple", __FILE__, __LINE__, "uh oh");
    return result;
}
