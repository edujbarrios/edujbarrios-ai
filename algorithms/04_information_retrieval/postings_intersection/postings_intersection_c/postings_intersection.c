#include "postings_intersection.h"
#include <stdint.h>

static bool is_valid(const uint32_t *values, size_t n) {
    if (n != 0 && !values) return false;
    for (size_t i = 0; i < n; ++i) {
        if (values[i] > INT32_MAX) return false;
        if (i > 0 && values[i] <= values[i-1]) return false;
    }
    return true;
}
bool postings_intersection(const uint32_t *left, size_t n_left,
                           const uint32_t *right, size_t n_right,
                           uint32_t *output, size_t *out_count) {
    if (!out_count || (n_left && n_right && !output) ||
        !is_valid(left,n_left) || !is_valid(right,n_right)) return false;
    size_t i = 0, j = 0, count = 0;
    while (i < n_left && j < n_right) {
        if (left[i] < right[j]) ++i;
        else if (left[i] > right[j]) ++j;
        else {
            output[count++] = left[i];
            ++i;
            ++j;
        }
    }
    *out_count = count;
    return true;
}
