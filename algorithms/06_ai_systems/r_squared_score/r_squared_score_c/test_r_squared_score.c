#include "r_squared_score.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1, 2, 3}, b[] = {1, 2, 2};
    const double zeros[] = {0, 0, 0}, same[] = {4, 4, 4};
    const double off[] = {4, 4, 0};
    const double extreme[] = {1e308, -1e308}, extremezero[] = {0, 0};
    const double nan[] = {NAN, 1};
    double out = -9;
    assert(r_squared_score(a, a, 3, &out) && out == 1);
    assert(r_squared_score(a, b, 3, &out) && fabs(out - 0.5) < 1e-12);
    assert(r_squared_score(a, zeros, 3, &out) && fabs(out + 6) < 1e-12);
    assert(r_squared_score(same, same, 3, &out) && out == 1);
    assert(r_squared_score(same, off, 3, &out) && out == 0);
    assert(r_squared_score(extreme, extremezero, 2, &out) && fabs(out) < 1e-12);
    out = -9;
    assert(!r_squared_score(NULL, a, 3, &out));
    assert(!r_squared_score(a, b, 1, &out));
    assert(!r_squared_score(a, b, 3, NULL));
    assert(!r_squared_score(nan, b, 2, &out));
    assert(out == -9);
    puts("C R-squared tests passed");
    return 0;
}
