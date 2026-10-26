#include "mean_absolute_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double y[] = {2.0, -1.0, 5.0};
    const double predictions[] = {1.0, 1.0, 3.0};
    const double invalid[] = {NAN};
    const double infinity[] = {INFINITY};
    const double huge[] = {1e308, 1e308};
    const double zeros[] = {0.0, 0.0};
    const double negative[] = {-1e308};
    double out = -9.0;
    assert(mean_absolute_error(y, predictions, 3, &out));
    assert(fabs(out - 5.0/3.0) < 1e-12);
    assert(mean_absolute_error(y, y, 3, &out) && out == 0.0);
    assert(mean_absolute_error(y, predictions, 1, &out) && out == 1.0);
    assert(mean_absolute_error(huge, zeros, 2, &out) && out == 1e308);
    assert(!mean_absolute_error(invalid, zeros, 1, &out) && out == 1e308);
    assert(!mean_absolute_error(infinity, zeros, 1, &out));
    assert(!mean_absolute_error(huge, negative, 1, &out));
    assert(!mean_absolute_error(NULL, zeros, 2, &out));
    assert(!mean_absolute_error(y, predictions, 0, &out));
    assert(!mean_absolute_error(y, predictions, 3, NULL));
    puts("C mean_absolute_error tests passed");
    return 0;
}
