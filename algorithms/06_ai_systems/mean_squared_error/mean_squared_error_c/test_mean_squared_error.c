#include "mean_squared_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double a[] = {3, 0}, b[] = {1, 2};
    const double same[] = {1, 2}, shifted[] = {2, 3};
    const double large[] = {1e154, 1e154}, zeros[] = {0, 0};
    const double overflow[] = {1e308}, nan[] = {NAN};
    double value = -9;
    assert(mean_squared_error(a, b, 2, &value) && value == 4);
    assert(mean_squared_error(same, same, 2, &value) && value == 0);
    assert(mean_squared_error(same, shifted, 2, &value) && value == 1);
    assert(mean_squared_error(large, zeros, 2, &value));
    assert(fabs(value / 1e308 - 1.0) < 1e-12);
    value = -9;
    assert(!mean_squared_error(NULL, b, 2, &value));
    assert(!mean_squared_error(a, b, 0, &value));
    assert(!mean_squared_error(a, b, 2, NULL));
    assert(!mean_squared_error(overflow, zeros, 1, &value));
    assert(!mean_squared_error(nan, zeros, 1, &value));
    assert(value == -9);
    puts("C mean squared error tests passed");
    return 0;
}
