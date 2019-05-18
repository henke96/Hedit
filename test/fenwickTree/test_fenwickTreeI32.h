#include "fenwickTree/fenwickTreeI32.h"
#include <malloc.h>
#include <assert.h>

void test_fenwicktreeI32(void) {
    struct fenwickTreeI32 *fenwickTree = malloc(
        fenwickTreeI32_SIZE(10)
    );

    fenwickTreeI32_init(fenwickTree, 10);

    fenwickTreeI32_add(fenwickTree, 0, 3);
    fenwickTreeI32_add(fenwickTree, 0, 1);
    fenwickTreeI32_add(fenwickTree, 2, 5);
    
    assert(fenwickTreeI32_sum(fenwickTree, 0) == 0);
    assert(fenwickTreeI32_sum(fenwickTree, 1) == 4);
    assert(fenwickTreeI32_sum(fenwickTree, 2) == 4);
    assert(fenwickTreeI32_sum(fenwickTree, 3) == 9);
    assert(fenwickTreeI32_sum(fenwickTree, 4) == 9);
    assert(fenwickTreeI32_sum(fenwickTree, 10) == 9);

    fenwickTreeI32_add(fenwickTree, 2, -3);
    fenwickTreeI32_add(fenwickTree, 5, -7);

    assert(fenwickTreeI32_sum(fenwickTree, 0) == 0);
    assert(fenwickTreeI32_sum(fenwickTree, 1) == 4);
    assert(fenwickTreeI32_sum(fenwickTree, 2) == 4);
    assert(fenwickTreeI32_sum(fenwickTree, 3) == 6);
    assert(fenwickTreeI32_sum(fenwickTree, 4) == 6);
    assert(fenwickTreeI32_sum(fenwickTree, 6) == -1);
    assert(fenwickTreeI32_sum(fenwickTree, 10) == -1);


    free(fenwickTree);
}