#include "optimal_string_alignment.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const int32_t a[] = {1, 2}, b[] = {2, 1};
    const int32_t c[] = {1, 2, 3}, d[] = {1, 3, 2};
    const int32_t restricted_a[] = {3, 1}, restricted_b[] = {1, 2, 3};
    const int32_t hi[] = {INT32_MAX}, lo[] = {INT32_MIN};
    size_t value = 99;
    assert(optimal_string_alignment(a, 2, b, 2, &value) && value == 1);
    assert(optimal_string_alignment(c, 3, d, 3, &value) && value == 1);
    assert(optimal_string_alignment(restricted_a, 2, restricted_b, 3, &value)
           && value == 3);
    assert(optimal_string_alignment(NULL, 0, a, 2, &value) && value == 2);
    assert(optimal_string_alignment(a, 2, NULL, 0, &value) && value == 2);
    assert(optimal_string_alignment(NULL, 0, NULL, 0, &value) && value == 0);
    assert(optimal_string_alignment(hi, 1, lo, 1, &value) && value == 1);
    assert(optimal_string_alignment(c, 3, c, 3, &value) && value == 0);
    value = 99;
    assert(!optimal_string_alignment(NULL, 1, a, 2, &value));
    assert(!optimal_string_alignment(a, 2, NULL, 1, &value));
    assert(!optimal_string_alignment(a, 2, b, 2, NULL));
    assert(value == 99);
    puts("C optimal string alignment tests passed");
    return 0;
}
