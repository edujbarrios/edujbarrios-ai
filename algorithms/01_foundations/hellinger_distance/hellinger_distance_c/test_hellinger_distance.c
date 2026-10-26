#include "hellinger_distance.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double p[] = {1, 0}, q[] = {0, 1}, r[] = {0.5, 0.5};
    const double bad[] = {0.4, 0.4}, negative[] = {1.1, -0.1};
    const double nan[] = {NAN, 0};
    double out = -9;
    assert(hellinger_distance(p, p, 2, &out) && out == 0);
    assert(hellinger_distance(p, q, 2, &out) && out == 1);
    assert(hellinger_distance(p, r, 2, &out));
    assert(fabs(out - sqrt(1 - 1 / sqrt(2))) < 1e-12);
    out = -9;
    assert(!hellinger_distance(p, q, 0, &out));
    assert(!hellinger_distance(NULL, q, 2, &out));
    assert(!hellinger_distance(p, q, 2, NULL));
    assert(!hellinger_distance(bad, p, 2, &out));
    assert(!hellinger_distance(negative, p, 2, &out));
    assert(!hellinger_distance(nan, p, 2, &out));
    assert(out == -9);
    puts("C Hellinger tests passed");
    return 0;
}
