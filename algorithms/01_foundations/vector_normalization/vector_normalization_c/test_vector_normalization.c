#include "vector_normalization.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {3.0, 4.0};
    const double negative[] = {-3.0, 4.0};
    const double zero[] = {0.0, 0.0};
    const double huge[] = {1e308, 1e308};
    const double tiny[] = {1e-308};
    const double bad[] = {NAN};
    const double inf[] = {INFINITY};
    double out[] = {-9.0, -9.0};

    assert(vector_normalization(a, 2, out));
    assert(fabs(out[0] - 0.6) < 1e-12 && fabs(out[1] - 0.8) < 1e-12);
    assert(vector_normalization(negative, 2, out));
    assert(fabs(out[0] + 0.6) < 1e-12 && fabs(out[1] - 0.8) < 1e-12);
    assert(vector_normalization(huge, 2, out));
    assert(fabs(hypot(out[0], out[1]) - 1.0) < 1e-12);
    assert(vector_normalization(tiny, 1, out) && out[0] == 1.0);

    out[0] = -9.0; out[1] = -9.0;
    assert(!vector_normalization(zero, 2, out) && out[0] == -9.0);
    assert(!vector_normalization(a, 0, out) && out[0] == -9.0);
    assert(!vector_normalization(bad, 1, out) && out[0] == -9.0);
    assert(!vector_normalization(inf, 1, out) && out[0] == -9.0);
    assert(!vector_normalization(a, 2, NULL));

    double in_place[] = {3.0, 4.0};
    assert(vector_normalization(in_place, 2, in_place));
    assert(fabs(in_place[0] - 0.6) < 1e-12);
    puts("C vector_normalization tests passed");
    return 0;
}
