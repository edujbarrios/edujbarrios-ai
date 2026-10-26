#include "balanced_accuracy.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const uint8_t truth[] = {0, 0, 0, 1, 1};
    const uint8_t pred[] = {0, 0, 1, 1, 0};
    const uint8_t all_zero[] = {0, 0, 0, 0, 0};
    const uint8_t valid[] = {0, 1};
    const uint8_t reversed[] = {1, 0};
    const uint8_t invalid[] = {0, 2};
    const uint8_t single[] = {1, 1};
    double result = -1.0;
    assert(balanced_accuracy(truth, pred, 5, &result));
    assert(fabs(result - 7.0 / 12.0) < 1e-12);
    assert(balanced_accuracy(valid, valid, 2, &result) && result == 1.0);
    assert(balanced_accuracy(valid, reversed, 2, &result) && result == 0.0);
    assert(balanced_accuracy(truth, all_zero, 5, &result) && result == 0.5);
    assert(!balanced_accuracy(single, single, 2, &result) && result == 0.5);
    assert(!balanced_accuracy(valid, invalid, 2, &result) && result == 0.5);
    assert(!balanced_accuracy(NULL, valid, 2, &result));
    assert(!balanced_accuracy(valid, valid, 0, &result));
    assert(!balanced_accuracy(valid, valid, 2, NULL));
    puts("C balanced_accuracy tests passed");
    return 0;
}
