#include "binary_connected_components.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    size_t labels[4096] = {777}, count = 777;
    const uint8_t mask[] = {1,1,0,0,1,0,0,0,1};
    assert(binary_connected_components(mask, 3, 3, labels, &count));
    const size_t expected[] = {1,1,0,0,1,0,0,0,2};
    assert(count == 2);
    for (size_t i = 0; i < 9; ++i) assert(labels[i] == expected[i]);
    const uint8_t diagonal[] = {1,0,0,1};
    assert(binary_connected_components(diagonal, 2, 2, labels, &count));
    assert(count == 2 && labels[0] == 1 && labels[3] == 2);
    const uint8_t whole[] = {1,1,1,1};
    assert(binary_connected_components(whole, 2, 2, labels, &count));
    assert(count == 1 && labels[0] == 1 && labels[3] == 1);
    const uint8_t zero[] = {0,0,0,0};
    assert(binary_connected_components(zero, 2, 2, labels, &count));
    assert(count == 0 && labels[0] == 0 && labels[3] == 0);
    const uint8_t line[] = {1,0,1};
    assert(binary_connected_components(line, 1, 3, labels, &count));
    assert(count == 2 && labels[0] == 1 && labels[1] == 0 && labels[2] == 2);
    assert(binary_connected_components(line, 3, 1, labels, &count));
    assert(count == 2 && labels[0] == 1 && labels[2] == 2);
    uint8_t checker[4096];
    for (size_t r = 0; r < 64; ++r)
        for (size_t c = 0; c < 64; ++c) checker[r * 64 + c] = (uint8_t)((r + c) % 2);
    assert(binary_connected_components(checker, 64, 64, labels, &count));
    assert(count == 2048);
    size_t next = 1;
    for (size_t i = 0; i < 4096; ++i) {
        if (checker[i]) assert(labels[i] == next++);
        else assert(labels[i] == 0);
    }
    labels[0] = 777; count = 777;
    const uint8_t invalid[] = {0,2};
    assert(!binary_connected_components(invalid, 1, 2, labels, &count));
    assert(!binary_connected_components(line, 0, 3, labels, &count));
    assert(!binary_connected_components(line, 1, 65, labels, &count));
    assert(!binary_connected_components(NULL, 1, 1, labels, &count));
    assert(!binary_connected_components(line, 1, 3, NULL, &count));
    assert(!binary_connected_components(line, 1, 3, labels, NULL));
    assert(labels[0] == 777 && count == 777);
    puts("C binary connected components tests passed");
    return 0;
}
