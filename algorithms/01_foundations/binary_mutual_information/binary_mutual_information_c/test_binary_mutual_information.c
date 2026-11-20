#include "binary_mutual_information.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const uint8_t a[] = {0,0,1,1}, b[] = {0,0,1,1};
    const uint8_t independent[] = {0,1,0,1};
    const uint8_t weak[] = {0,0,0,1};
    const uint8_t bad[] = {0,2,1,1};
    const uint8_t constant[] = {0,0,0,0};
    double result = -9;
    assert(binary_mutual_information(a, b, 4, &result));
    assert(fabs(result - 1.0) < 1e-12);
    assert(binary_mutual_information(a, independent, 4, &result));
    assert(fabs(result) < 1e-12);
    assert(binary_mutual_information(a, weak, 4, &result));
    assert(fabs(result - 0.31127812445913283) < 1e-12);
    assert(binary_mutual_information(constant, a, 4, &result));
    assert(result == 0.0);
    result = -9;
    assert(!binary_mutual_information(a, bad, 4, &result));
    assert(!binary_mutual_information(a, b, 0, &result));
    assert(!binary_mutual_information(a, b, 4, NULL));
    assert(result == -9);
    puts("C binary mutual information tests passed");
    return 0;
}
