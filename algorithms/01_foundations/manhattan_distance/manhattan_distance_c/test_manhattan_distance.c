#include "manhattan_distance.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1.0, 2.0, -1.0};
    const double b[] = {4.0, -2.0, 1.0};
    const double invalid[] = {NAN};
    const double huge[] = {1e308, 1e308};
    const double negative[] = {-1e308};
    const double zeros[] = {0.0, 0.0};
    double out = -7.0;
    assert(manhattan_distance(a, b, 3, &out) && out == 9.0);
    assert(manhattan_distance(a, a, 3, &out) && out == 0.0);
    assert(manhattan_distance(NULL, NULL, 0, &out) && out == 0.0);
    assert(manhattan_distance(b, a, 3, &out) && out == 9.0);
    assert(!manhattan_distance(invalid, zeros, 1, &out) && out == 9.0);
    assert(!manhattan_distance(huge, zeros, 2, &out) && out == 9.0);
    assert(!manhattan_distance(huge, negative, 1, &out) && out == 9.0);
    assert(!manhattan_distance(a, b, 3, NULL));
    assert(!manhattan_distance(NULL, b, 1, &out));
    puts("C manhattan_distance tests passed");
    return 0;
}
