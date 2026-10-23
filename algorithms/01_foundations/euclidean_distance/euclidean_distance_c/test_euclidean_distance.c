#include "euclidean_distance.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
    const double origin[] = {0.0, 0.0};
    const double other[] = {3.0, 4.0};
    const double nan_value[] = {NAN};
    const double huge[] = {1e154};
    const double overflow_positive[] = {1e308};
    const double overflow_negative[] = {-1e308};
    double out = -1.0;

    assert(euclidean_distance(origin, other, 2, &out) && out == 5.0);
    assert(euclidean_distance(other, other, 2, &out) && out == 0.0);
    assert(euclidean_distance(NULL, NULL, 0, &out) && out == 0.0);
    assert(euclidean_distance(huge, overflow_negative, 0, &out) && out == 0.0);
    assert(euclidean_distance(huge, origin, 1, &out) && isfinite(out));
    assert(fabs(out / 1e154 - 1.0) < 1e-12);
    assert(!euclidean_distance(nan_value, origin, 1, &out));
    assert(!euclidean_distance(overflow_positive, overflow_negative, 1, &out));
    assert(!euclidean_distance(origin, other, 2, NULL));
    puts("C euclidean_distance tests passed");
    return 0;
}
