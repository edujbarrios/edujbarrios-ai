#include "canberra_distance.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {1, 2}, y[] = {2, 4}, zero[] = {0, 0};
    const double opposite[] = {-1, 1}, signs[] = {1, -1};
    const double huge[] = {1e308, -1e308}, reversed[] = {-1e308, 1e308};
    const double invalid[] = {NAN};
    double result = -9;
    assert(canberra_distance(x, y, 2, &result));
    assert(fabs(result - 2.0 / 3.0) < 1e-12);
    assert(canberra_distance(zero, zero, 2, &result) && result == 0);
    assert(canberra_distance(signs, opposite, 2, &result) && result == 2);
    assert(canberra_distance(huge, reversed, 2, &result) && result == 2);
    result = -9;
    assert(!canberra_distance(x, y, 0, &result));
    assert(!canberra_distance(NULL, y, 2, &result));
    assert(!canberra_distance(x, y, 2, NULL));
    assert(!canberra_distance(invalid, y, 1, &result));
    assert(result == -9);
    puts("C Canberra distance tests passed");
    return 0;
}
