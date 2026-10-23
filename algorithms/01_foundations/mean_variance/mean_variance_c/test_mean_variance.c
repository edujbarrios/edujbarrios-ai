#include "mean_variance.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double data[] = {2, 4, 4, 4, 5, 5, 7, 9};
    const double constant[] = {7, 7, 7};
    const double negative[] = {-2, 0, 2};
    const double bad[] = {NAN};
    const double extreme[] = {1e308, -1e308};
    double mean = -100.0, variance = -100.0;

    assert(mean_variance(data, 8, &mean, &variance));
    assert(fabs(mean - 5.0) < 1e-12 && fabs(variance - 4.0) < 1e-12);
    assert(mean_variance(constant, 3, &mean, &variance));
    assert(mean == 7.0 && variance == 0.0);
    assert(mean_variance(negative, 3, &mean, &variance));
    assert(fabs(mean) < 1e-12 && fabs(variance - 8.0 / 3.0) < 1e-12);
    mean = -100.0; variance = -100.0;
    assert(!mean_variance(data, 0, &mean, &variance));
    assert(!mean_variance(bad, 1, &mean, &variance));
    assert(!mean_variance(extreme, 2, &mean, &variance));
    assert(mean == -100.0 && variance == -100.0);
    assert(!mean_variance(data, 8, &mean, &mean));
    puts("C mean_variance tests passed");
    return 0;
}
