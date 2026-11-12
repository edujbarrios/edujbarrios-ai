#include "damerau_levenshtein_distance.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint8_t ca[] = {67, 65}, abc[] = {65, 66, 67};
    const uint8_t ab[] = {1, 2}, ba[] = {2, 1};
    const uint8_t repeated[] = {1, 1, 2}, shifted[] = {1, 2, 1};
    const uint8_t high[] = {0, 255}, reversed[] = {255, 0};
    const uint8_t x[] = {1, 2, 3}, y[] = {1, 4, 3};
    size_t result = 999;
    assert(damerau_levenshtein_distance(ca, 2, abc, 3, &result) && result == 2);
    assert(damerau_levenshtein_distance(abc, 3, ca, 2, &result) && result == 2);
    assert(damerau_levenshtein_distance(ab, 2, ba, 2, &result) && result == 1);
    assert(damerau_levenshtein_distance(repeated, 3, shifted, 3, &result) && result == 1);
    assert(damerau_levenshtein_distance(high, 2, reversed, 2, &result) && result == 1);
    assert(damerau_levenshtein_distance(x, 3, y, 3, &result) && result == 1);
    assert(damerau_levenshtein_distance(x, 3, x, 3, &result) && result == 0);
    assert(damerau_levenshtein_distance(NULL, 0, high, 2, &result) && result == 2);
    assert(damerau_levenshtein_distance(high, 2, NULL, 0, &result) && result == 2);
    assert(damerau_levenshtein_distance(NULL, 0, NULL, 0, &result) && result == 0);
    uint8_t same[256] = {0};
    assert(damerau_levenshtein_distance(same, 256, same, 256, &result) && result == 0);
    result = 999;
    assert(!damerau_levenshtein_distance(NULL, 1, ab, 2, &result));
    assert(!damerau_levenshtein_distance(ab, 2, NULL, 1, &result));
    assert(!damerau_levenshtein_distance(ab, 257, ba, 2, &result));
    assert(!damerau_levenshtein_distance(ab, 2, ba, 2, NULL));
    assert(result == 999);
    puts("C unrestricted Damerau-Levenshtein tests passed");
    return 0;
}
