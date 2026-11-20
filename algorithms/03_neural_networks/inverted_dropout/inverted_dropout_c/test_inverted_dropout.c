#include "inverted_dropout.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double activations[] = {2.0, -4.0, 0.0};
    const uint8_t mask[] = {1, 0, 1};
    const double huge[] = {1.0e308};
    const double bad[] = {1.0, NAN};
    const uint8_t one[] = {1}, zero[] = {0}, bad_mask[] = {1, 2};
    double output[] = {-99.0, -99.0, -99.0};
    assert(inverted_dropout(activations, mask, 3, 0.5, output));
    assert(output[0] == 4.0 && output[1] == 0.0 && output[2] == 0.0);
    assert(inverted_dropout(huge, zero, 1, 0.99, output));
    assert(output[0] == 0.0);
    assert(inverted_dropout(activations, mask, 0, 0.5, output));
    output[0] = -99.0;
    assert(!inverted_dropout(huge, one, 1, 0.99, output));
    assert(!inverted_dropout(bad, mask, 2, 0.5, output));
    assert(!inverted_dropout(activations, bad_mask, 2, 0.5, output));
    assert(!inverted_dropout(activations, mask, 3, 1.0, output));
    assert(!inverted_dropout(activations, mask, 3, -0.1, output));
    assert(!inverted_dropout(activations, mask, 3, NAN, output));
    assert(!inverted_dropout(activations, mask, 3, 0.5, NULL));
    assert(output[0] == -99.0);
    puts("C inverted dropout tests passed");
    return 0;
}
