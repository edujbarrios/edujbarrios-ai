#include "hamming_top_k.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const uint32_t values[] = {1, 7, 15, 2, 3};
    const uint32_t duplicate[] = {3, 3, 0};
    const uint32_t invalid[] = {3, UINT32_MAX};
    const uint32_t extremes[] = {0, (UINT32_C(1) << 30) - 1};
    size_t out[] = {99, 99, 99, 99, 99};
    assert(hamming_top_k(3, values, 5, 4, out));
    assert(out[0] == 4 && out[1] == 0 && out[2] == 1 && out[3] == 3);
    assert(hamming_top_k(3, duplicate, 3, 2, out));
    assert(out[0] == 0 && out[1] == 1);
    assert(hamming_top_k(0, extremes, 2, 2, out));
    assert(out[0] == 0 && out[1] == 1);
    out[0] = 99;
    assert(!hamming_top_k(3, invalid, 2, 1, out));
    assert(!hamming_top_k(UINT32_MAX, values, 5, 1, out));
    assert(!hamming_top_k(3, values, 5, 0, out));
    assert(!hamming_top_k(3, values, 5, 6, out));
    assert(!hamming_top_k(3, NULL, 5, 1, out));
    assert(!hamming_top_k(3, values, 5, 1, NULL));
    assert(out[0] == 99);
    puts("C hamming_top_k tests passed");
    return 0;
}
