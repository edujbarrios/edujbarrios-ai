#include "dot_product.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
    const double a[] = {1.0, 2.0, 3.0};
    const double b[] = {4.0, 5.0, 6.0};
    const double neg[] = {-1.0, 2.0, -3.0};
    const double mixed[] = {4.0, -5.0, 6.0};
    const double bad[] = {NAN};
    const double huge[] = {1e308};
    double out = -1.0;

    assert(dot_product(a, b, 3, &out) && out == 32.0);
    assert(dot_product(neg, mixed, 3, &out) && out == -32.0);
    assert(dot_product(NULL, NULL, 0, &out) && out == 0.0);
    assert(!dot_product(NULL, b, 3, &out) && out == 0.0);
    assert(!dot_product(bad, b, 1, &out) && out == 0.0);
    assert(!dot_product(huge, huge, 1, &out) && out == 0.0);
    assert(!dot_product(a, b, 3, NULL));
    puts("C dot_product tests passed");
    return 0;
}
