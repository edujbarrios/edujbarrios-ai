#include "longest_common_subsequence.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const int32_t a[] = {1, 2, 3, 4}, b[] = {2, 4, 3};
    const int32_t c[] = {1, 1, 2}, d[] = {1, 2, 1};
    const int32_t hi[] = {INT32_MAX}, lo[] = {INT32_MIN};
    size_t length = 999;
    assert(longest_common_subsequence(a, 4, b, 3, &length) && length == 2);
    assert(longest_common_subsequence(b, 3, a, 4, &length) && length == 2);
    assert(longest_common_subsequence(c, 3, d, 3, &length) && length == 2);
    assert(longest_common_subsequence(NULL, 0, a, 4, &length) && length == 0);
    assert(longest_common_subsequence(a, 4, NULL, 0, &length) && length == 0);
    assert(longest_common_subsequence(NULL, 0, NULL, 0, &length) && length == 0);
    assert(longest_common_subsequence(hi, 1, hi, 1, &length) && length == 1);
    assert(longest_common_subsequence(hi, 1, lo, 1, &length) && length == 0);
    length = 999;
    assert(!longest_common_subsequence(NULL, 1, a, 4, &length));
    assert(!longest_common_subsequence(a, 4, NULL, 1, &length));
    assert(!longest_common_subsequence(a, 4, b, 3, NULL));
    assert(length == 999);
    puts("C LCS tests passed");
    return 0;
}
