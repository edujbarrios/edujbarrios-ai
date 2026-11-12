#include "isotonic_regression.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void near(const double *a, const double *b, size_t n) {
    for (size_t i = 0; i < n; ++i) assert(fabs(a[i] - b[i]) < 1e-9);
}
int main(void) {
    const double a[] = {3, 1, 2, 4}, expected[] = {2, 2, 2, 4};
    const double b[] = {-2, 2, 0, 4}, be[] = {-2, 1, 1, 4};
    const double inc[] = {1, 2, 2, 3}, dec[] = {3, 2, 1};
    const double de[] = {2, 2, 2}, lim[] = {1000000, -1000000};
    const double lie[] = {0, 0};
    double out[512] = {0};
    assert(isotonic_regression(a, 4, out)); near(out, expected, 4);
    assert(isotonic_regression(b, 4, out)); near(out, be, 4);
    assert(isotonic_regression(inc, 4, out)); near(out, inc, 4);
    assert(isotonic_regression(dec, 3, out)); near(out, de, 3);
    assert(isotonic_regression(lim, 2, out)); near(out, lie, 2);
    double inplace[] = {3, 1, 2, 4};
    assert(isotonic_regression(inplace, 4, inplace)); near(inplace, expected, 4);
    double descending[512];
    for (size_t i = 0; i < 512; ++i) descending[i] = 512.0 - (double)i;
    assert(isotonic_regression(descending, 512, out));
    for (size_t i = 0; i < 512; ++i) assert(fabs(out[i] - 256.5) < 1e-9);
    const double invalid[] = {1, NAN}, huge[] = {1000001};
    out[0] = 77;
    assert(!isotonic_regression(NULL, 1, out));
    assert(!isotonic_regression(a, 0, out));
    assert(!isotonic_regression(a, 513, out));
    assert(!isotonic_regression(a, 4, NULL));
    assert(!isotonic_regression(invalid, 2, out));
    assert(!isotonic_regression(huge, 1, out));
    assert(out[0] == 77);
    puts("C isotonic regression tests passed");
    return 0;
}
