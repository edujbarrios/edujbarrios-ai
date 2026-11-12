#include "damerau_levenshtein_distance.h"
#include <stdlib.h>

static size_t minimum(size_t x, size_t y) { return x < y ? x : y; }

bool damerau_levenshtein_distance(const uint8_t *a, size_t n,
                                  const uint8_t *b, size_t m, size_t *output) {
    if (!output || n > 256 || m > 256 || (n && !a) || (m && !b))
        return false;
    const size_t cols = m + 2;
    size_t *distance = calloc((n + 2) * cols, sizeof(*distance));
    if (!distance) return false;
#define H(i, j) distance[(i) * cols + (j)]
    const size_t bound = n + m;
    H(0, 0) = bound;
    for (size_t i = 0; i <= n; ++i) {
        H(i + 1, 0) = bound;
        H(i + 1, 1) = i;
    }
    for (size_t j = 0; j <= m; ++j) {
        H(0, j + 1) = bound;
        H(1, j + 1) = j;
    }
    size_t seen[256] = {0};
    for (size_t i = 1; i <= n; ++i) {
        size_t last_match = 0;
        for (size_t j = 1; j <= m; ++j) {
            const size_t previous_a = seen[b[j - 1]];
            const size_t previous_b = last_match;
            const size_t cost = a[i - 1] == b[j - 1] ? 0 : 1;
            if (cost == 0) last_match = j;
            const size_t transposition =
                H(previous_a, previous_b) + (i - previous_a - 1) +
                1 + (j - previous_b - 1);
            H(i + 1, j + 1) =
                minimum(minimum(H(i, j) + cost, H(i + 1, j) + 1),
                        minimum(H(i, j + 1) + 1, transposition));
        }
        seen[a[i - 1]] = i;
    }
    *output = H(n + 1, m + 1);
#undef H
    free(distance);
    return true;
}
