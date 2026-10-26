#include "overlap_coefficient.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const int32_t a[] = {1, 1, 2}, b[] = {1, 2, 3, 4};
    const int32_t c[] = {1, 2}, d[] = {2, 3};
    const int32_t low[] = {INT32_MIN}, limits[] = {INT32_MAX, INT32_MIN};
    double result = -1.0;
    assert(overlap_coefficient(a, 3, b, 4, &result) && result == 1);
    assert(overlap_coefficient(b, 4, a, 3, &result) && result == 1);
    assert(overlap_coefficient(c, 2, d, 2, &result) && result == 0.5);
    assert(overlap_coefficient(low, 1, limits, 2, &result) && result == 1);
    assert(overlap_coefficient(NULL, 0, NULL, 0, &result) && result == 1);
    assert(overlap_coefficient(NULL, 0, a, 3, &result) && result == 0);
    assert(overlap_coefficient(a, 3, NULL, 0, &result) && result == 0);
    result = -1;
    assert(!overlap_coefficient(NULL, 1, b, 4, &result));
    assert(!overlap_coefficient(a, 3, NULL, 1, &result));
    assert(!overlap_coefficient(a, 3, b, 4, NULL));
    assert(result == -1);
    puts("C overlap coefficient tests passed");
    return 0;
}
