#include "median_absolute_deviation.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *v, size_t n, double expected) {
    double result = -1;
    assert(median_absolute_deviation(v, n, &result));
    assert(result == expected);
}

int main(void) {
    const double odd[] = {1, 1, 2, 2, 4, 6, 9};
    const double even[] = {1, 2, 3, 10};
    const double reverse[] = {10, 3, 2, 1};
    const double same[] = {5, 5, 5};
    const double one[] = {42};
    const double neg[] = {-3, -1, 2};
    const double large[] = {-1e308, 1e308};
    check(odd, 7, 1);
    check(even, 4, 1);
    check(reverse, 4, 1);
    check(same, 3, 0);
    check(one, 1, 0);
    check(neg, 3, 2);
    check(large, 2, 1e308);
    double big[513];
    for (size_t i = 0; i < 513; ++i) big[i] = 7.0;
    check(big, 512, 0);
    double result = 99.0;
    assert(!median_absolute_deviation(big, 513, &result));
    assert(!median_absolute_deviation(big, 0, &result));
    assert(!median_absolute_deviation(NULL, 1, &result));
    assert(!median_absolute_deviation(big, 1, NULL));
    const double invalid[] = {1.0, NAN};
    assert(!median_absolute_deviation(invalid, 2, &result));
    const double infinite[] = {1.0, INFINITY};
    assert(!median_absolute_deviation(infinite, 2, &result));
    const double overflow[] = {-1e308, -1e308, 1e308};
    assert(!median_absolute_deviation(overflow, 3, &result));
    assert(result == 99.0);
    puts("C median absolute deviation tests passed");
    return 0;
}
