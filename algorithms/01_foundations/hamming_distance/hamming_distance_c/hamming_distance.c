#include "hamming_distance.h"

bool hamming_distance(const uint8_t *a, size_t len_a, const uint8_t *b,
                      size_t len_b, size_t *distance) {
    if (!distance || len_a != len_b ||
        (len_a != 0 && (!a || !b))) {
        return false;
    }
    size_t count = 0;
    for (size_t i = 0; i < len_a; ++i) {
        if (a[i] != b[i]) ++count;
    }
    *distance = count;
    return true;
}
