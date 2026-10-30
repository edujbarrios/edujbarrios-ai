#include "sparsemax.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

static void close_to(double actual, double expected) {
    assert(fabs(actual - expected) < 1e-12);
}

int main(void) {
    double out[] = {-9.0, -9.0, -9.0};
    const double tied[] = {0.0, 0.0, -1.0};
    const double dominant[] = {3.0, 1.0, 0.0};
    const double shifted[] = {100.0, 100.0, 99.0};
    const double bad[] = {0.0, NAN};
    const double infinite[] = {INFINITY};

    assert(sparsemax(tied, 3, out));
    close_to(out[0], 0.5);
    close_to(out[1], 0.5);
    close_to(out[2], 0.0);
    assert(sparsemax(dominant, 3, out));
    close_to(out[0], 1.0);
    close_to(out[1], 0.0);
    close_to(out[2], 0.0);
    assert(sparsemax(shifted, 3, out));
    close_to(out[0], 0.5);
    close_to(out[1], 0.5);
    close_to(out[2], 0.0);

    double inplace[] = {1.0, 1.0, 1.0};
    assert(sparsemax(inplace, 3, inplace));
    for (size_t i = 0; i < 3; ++i) close_to(inplace[i], 1.0 / 3.0);
    const double extreme[] = {1e308, -1e308};
    assert(sparsemax(extreme, 2, out));
    close_to(out[0], 1.0);
    close_to(out[1], 0.0);

    out[0] = -9.0;
    assert(!sparsemax(bad, 2, out) && out[0] == -9.0);
    assert(!sparsemax(infinite, 1, out) && out[0] == -9.0);
    assert(!sparsemax(tied, 0, out));
    assert(!sparsemax(NULL, 3, out));
    assert(!sparsemax(tied, 3, NULL));
    puts("C sparsemax tests passed");
    return 0;
}
