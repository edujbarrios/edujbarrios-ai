#include "softplus.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {0, 1, -1, 1000, -1000}, bad[] = {1, NAN};
    double v[] = {-9, -9, -9, -9, -9}, d[] = {-9, -9, -9, -9, -9};
    assert(softplus(x, 5, v, d));
    assert(fabs(v[0] - log(2)) < 1e-12 && d[0] == 0.5);
    assert(fabs(v[1] - log1p(exp(1))) < 1e-12);
    assert(fabs(v[2] - log1p(exp(-1))) < 1e-12);
    assert(fabs(d[1] - 1.0 / (1.0 + exp(-1))) < 1e-12);
    assert(fabs(d[2] - 1.0 / (1.0 + exp(1))) < 1e-12);
    assert(v[3] == 1000 && d[3] == 1);
    assert(v[4] == 0 && d[4] == 0);
    v[0] = -9; d[0] = -9;
    assert(!softplus(NULL, 5, v, d));
    assert(!softplus(x, 0, v, d));
    assert(!softplus(x, 5, NULL, d));
    assert(!softplus(bad, 2, v, d));
    assert(v[0] == -9 && d[0] == -9);
    puts("C Softplus tests passed");
    return 0;
}
