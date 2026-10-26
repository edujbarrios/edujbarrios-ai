#include "tversky_index.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const int32_t a[] = {1, 2}, b[] = {2, 3, 4}, dup[] = {1, 1, 2};
    const int32_t min[] = {INT32_MIN}, bounds[] = {INT32_MIN, INT32_MAX};
    double out = -9;
    assert(tversky_index(a, 2, b, 3, 1, 1, &out) && out == 0.25);
    assert(tversky_index(a, 2, b, 3, 0.5, 2, &out));
    assert(fabs(out - 2.0/11.0) < 1e-12);
    assert(tversky_index(b, 3, a, 2, 0.5, 2, &out) && out == 0.25);
    assert(tversky_index(dup, 3, a, 2, 1, 1, &out) && out == 1);
    assert(tversky_index(NULL, 0, NULL, 0, 1, 1, &out) && out == 1);
    assert(tversky_index(NULL, 0, a, 2, 1, 1, &out) && out == 0);
    assert(tversky_index(min, 1, bounds, 2, 1, 1, &out) && out == 0.5);
    out = -9;
    assert(!tversky_index(NULL, 1, b, 3, 1, 1, &out));
    assert(!tversky_index(a, 2, b, 3, 0, 1, &out));
    assert(!tversky_index(a, 2, b, 3, 1, NAN, &out));
    assert(!tversky_index(a, 2, b, 3, 1e308, 1e308, &out));
    assert(!tversky_index(a, 2, b, 3, 1, 1, NULL));
    assert(out == -9);
    puts("C Tversky tests passed");
    return 0;
}
