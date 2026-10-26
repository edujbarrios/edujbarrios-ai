#include "bray_curtis_dissimilarity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1, 2}, b[] = {2, 4}, z[] = {0, 0};
    const double x[] = {0, 1}, y[] = {1, 0};
    const double huge[] = {1e308, 1e308}, half[] = {1e308, 0};
    const double invalid[] = {-1}, nan[] = {NAN};
    double value = -9;
    assert(bray_curtis_dissimilarity(a, b, 2, &value));
    assert(fabs(value - 1.0 / 3.0) < 1e-12);
    assert(bray_curtis_dissimilarity(b, a, 2, &value));
    assert(fabs(value - 1.0 / 3.0) < 1e-12);
    assert(bray_curtis_dissimilarity(z, z, 2, &value) && value == 0);
    assert(bray_curtis_dissimilarity(x, y, 2, &value) && value == 1);
    assert(bray_curtis_dissimilarity(huge, half, 2, &value));
    assert(fabs(value - 1.0 / 3.0) < 1e-12);
    value = -9;
    assert(!bray_curtis_dissimilarity(NULL, a, 2, &value));
    assert(!bray_curtis_dissimilarity(a, b, 0, &value));
    assert(!bray_curtis_dissimilarity(a, b, 2, NULL));
    assert(!bray_curtis_dissimilarity(invalid, z, 1, &value));
    assert(!bray_curtis_dissimilarity(nan, z, 1, &value));
    assert(value == -9);
    puts("C Bray-Curtis dissimilarity tests passed");
    return 0;
}
