#include "minkowski_distance.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {0, 0}, b[] = {3, 4}, huge[] = {1e150, 1e150};
    const double low[] = {-1e308}, high[] = {1e308}, bad[] = {NAN};
    double out = -9;
    assert(minkowski_distance(a, b, 2, 1, &out) && out == 7);
    assert(minkowski_distance(a, b, 2, 2, &out) && out == 5);
    assert(minkowski_distance(a, b, 2, 3, &out));
    assert(fabs(out - pow(91, 1.0/3)) < 1e-12);
    assert(minkowski_distance(a, huge, 2, 2, &out));
    assert(fabs(out / 1e150 - sqrt(2)) < 1e-12);
    assert(minkowski_distance(a, a, 2, 2, &out) && out == 0);
    out = -9;
    assert(!minkowski_distance(NULL, b, 2, 2, &out));
    assert(!minkowski_distance(a, b, 0, 2, &out));
    assert(!minkowski_distance(a, b, 2, INFINITY, &out));
    assert(!minkowski_distance(a, b, 2, 0, &out));
    assert(!minkowski_distance(a, b, 2, 2, NULL));
    assert(!minkowski_distance(bad, a, 1, 2, &out));
    assert(!minkowski_distance(low, high, 1, 2, &out));
    assert(out == -9);
    puts("C Minkowski tests passed");
    return 0;
}
