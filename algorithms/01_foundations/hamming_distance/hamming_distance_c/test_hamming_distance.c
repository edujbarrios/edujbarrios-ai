#include "hamming_distance.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint8_t a[] = {1, 0, 1, 1};
    const uint8_t b[] = {1, 1, 0, 1};
    const uint8_t zero[] = {0};
    const uint8_t all_bits[] = {255};
    size_t distance = 999;

    assert(hamming_distance(a, 4, b, 4, &distance) && distance == 2);
    assert(hamming_distance(a, 4, a, 4, &distance) && distance == 0);
    assert(hamming_distance(zero, 1, all_bits, 1, &distance) && distance == 1);
    assert(hamming_distance(NULL, 0, NULL, 0, &distance) && distance == 0);
    distance = 999;
    assert(!hamming_distance(a, 4, b, 3, &distance) && distance == 999);
    assert(!hamming_distance(NULL, 4, b, 4, &distance) && distance == 999);
    assert(!hamming_distance(a, 4, NULL, 4, &distance) && distance == 999);
    assert(!hamming_distance(a, 4, b, 4, NULL));
    puts("C Hamming distance tests passed");
    return 0;
}
