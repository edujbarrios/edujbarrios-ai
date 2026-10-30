#include "binary_closing.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    uint8_t input[49] = {0}, expected[49] = {0}, output[49] = {0};
    for (size_t y = 2; y <= 4; ++y) for (size_t x = 2; x <= 4; ++x)
        input[y*7+x] = expected[y*7+x] = 1;
    input[3*7+3] = 0; /* internal hole */
    assert(binary_closing(input, 49, 7, 7, output));
    for (size_t i = 0; i < 49; ++i) assert(output[i] == expected[i]);
    assert(binary_closing(input, 49, 7, 7, input));
    for (size_t i = 0; i < 49; ++i) assert(input[i] == expected[i]);
    uint8_t one[] = {1}, zero[] = {0}, bad[] = {2}, out[] = {9};
    assert(binary_closing(one, 1, 1, 1, out) && out[0] == 0);
    assert(binary_closing(zero, 1, 1, 1, out) && out[0] == 0);
    out[0] = 9;
    assert(!binary_closing(bad, 1, 1, 1, out) && out[0] == 9);
    assert(!binary_closing(one, 0, 1, 1, out) && out[0] == 9);
    assert(!binary_closing(one, 1, 0, 1, out) && out[0] == 9);
    assert(!binary_closing(one, 1, 65, 1, out) && out[0] == 9);
    assert(!binary_closing(NULL, 1, 1, 1, out) && out[0] == 9);
    assert(!binary_closing(one, 1, 1, 1, NULL));
    puts("C binary closing tests passed");
    return 0;
}
