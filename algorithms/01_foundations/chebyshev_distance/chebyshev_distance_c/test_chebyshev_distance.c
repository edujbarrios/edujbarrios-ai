#include "chebyshev_distance.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1.0, 2.0, -1.0};
    const double b[] = {4.0, -2.0, 1.0};
    const double nan_value[] = {NAN};
    const double infinity[] = {INFINITY};
    const double huge[] = {1e308};
    const double negative[] = {-1e308};
    double result = -99.0;

    assert(chebyshev_distance(a, b, 3, &result) && result == 4.0);
    assert(chebyshev_distance(b, a, 3, &result) && result == 4.0);
    assert(chebyshev_distance(a, a, 3, &result) && result == 0.0);
    assert(chebyshev_distance(NULL, NULL, 0, &result) && result == 0.0);
    assert(chebyshev_distance(huge, a, 1, &result) && isfinite(result));
    assert(!chebyshev_distance(huge, negative, 1, &result));
    assert(!chebyshev_distance(nan_value, a, 1, &result));
    assert(!chebyshev_distance(infinity, a, 1, &result));
    assert(!chebyshev_distance(NULL, a, 1, &result));
    assert(!chebyshev_distance(a, b, 3, NULL));
    puts("C chebyshev_distance tests passed");
    return 0;
}
