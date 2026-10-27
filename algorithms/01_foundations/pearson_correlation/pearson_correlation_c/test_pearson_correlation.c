#include "pearson_correlation.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {1, 2, 3};
    const double positive[] = {2, 4, 6};
    const double negative[] = {6, 4, 2};
    const double independent[] = {1, 0, 1};
    const double constant[] = {4, 4, 4};
    const double invalid[] = {1, NAN, 3};
    const double high[] = {1, 1000001, 3};
    const double two[] = {1, 2};
    double out = 99.0;
    assert(pearson_correlation(x, positive, 3, &out) && fabs(out - 1.0) < 1e-12);
    assert(pearson_correlation(x, negative, 3, &out) && fabs(out + 1.0) < 1e-12);
    assert(pearson_correlation(x, independent, 3, &out) && fabs(out) < 1e-12);
    assert(pearson_correlation(two, two, 2, &out) && fabs(out - 1.0) < 1e-12);
    out = 99.0;
    assert(!pearson_correlation(x, constant, 3, &out));
    assert(!pearson_correlation(x, invalid, 3, &out));
    assert(!pearson_correlation(x, high, 3, &out));
    assert(!pearson_correlation(x, positive, 1, &out));
    assert(!pearson_correlation(x, positive, 10001, &out));
    assert(!pearson_correlation(NULL, positive, 3, &out));
    assert(!pearson_correlation(x, positive, 3, NULL));
    assert(out == 99.0);
    puts("C Pearson correlation tests passed");
    return 0;
}
