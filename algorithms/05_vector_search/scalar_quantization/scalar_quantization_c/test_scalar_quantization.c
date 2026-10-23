#include "scalar_quantization.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
int main(void) {
    double a[] = {0.0, 0.5, 1.0, -3.0, 8.0};
    uint8_t out[] = {9,9,9,9,9};
    assert(scalar_quantization(a, 5, 0.0, 1.0, out));
    assert(out[0]==0 && out[1]==128 && out[2]==255 && out[3]==0 && out[4]==255);
    assert(scalar_quantization(NULL, 0, 0.0, 1.0, out));
    out[0]=9;
    const double bad[] = {0.4, NAN};
    assert(!scalar_quantization(bad, 2, 0.0, 1.0, out) && out[0]==9);
    assert(!scalar_quantization(a, 5, 1.0, 1.0, out));
    assert(!scalar_quantization(a, 5, -1e308, 1e308, out));
    assert(!scalar_quantization(a, 5, 0.0, INFINITY, out));
    assert(!scalar_quantization(a, 5, 0.0, 1.0, NULL));
    assert(out[0]==9);
    puts("C scalar quantization tests passed");
    return 0;
}
