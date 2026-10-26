#include "exponential_moving_average.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double values[] = {10.0, 20.0, 10.0};
    const double extremes[] = {1e308, -1e308};
    const double invalid[] = {10.0, NAN, 20.0};
    double out[] = {-1.0, -1.0, -1.0};
    assert(exponential_moving_average(values, 3, 0.5, out));
    assert(out[0] == 10.0 && out[1] == 15.0 && out[2] == 12.5);
    assert(exponential_moving_average(values, 3, 1.0, out));
    assert(out[0] == 10.0 && out[1] == 20.0 && out[2] == 10.0);
    assert(exponential_moving_average(extremes, 2, 0.5, out));
    assert(isfinite(out[1]) && out[1] == 0.0);
    out[0] = -7.0; out[1] = -7.0; out[2] = -7.0;
    assert(!exponential_moving_average(invalid, 3, 0.5, out));
    assert(out[0] == -7.0 && out[1] == -7.0 && out[2] == -7.0);
    assert(!exponential_moving_average(values, 3, 0.0, out));
    assert(!exponential_moving_average(values, 3, 1.1, out));
    assert(!exponential_moving_average(values, 3, NAN, out));
    assert(!exponential_moving_average(values, 0, 0.5, out));
    assert(!exponential_moving_average(NULL, 3, 0.5, out));
    assert(!exponential_moving_average(values, 3, 0.5, NULL));
    puts("C exponential_moving_average tests passed");
    return 0;
}
