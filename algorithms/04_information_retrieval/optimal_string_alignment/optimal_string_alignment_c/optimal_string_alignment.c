#include "optimal_string_alignment.h"
#include <stdint.h>
#include <stdlib.h>

static size_t minimum(size_t a, size_t b) {
    return a < b ? a : b;
}

bool optimal_string_alignment(const int32_t *a, size_t n,
                              const int32_t *b, size_t m, size_t *result) {
    if (!result || (n > 0 && !a) || (m > 0 && !b)) return false;
    if (n == 0 || m == 0) {
        *result = n == 0 ? m : n;
        return true;
    }
    if (m > n) {
        const int32_t *tmp = a; a = b; b = tmp;
        const size_t tmp_len = n; n = m; m = tmp_len;
    }
    if (m >= SIZE_MAX / sizeof(size_t)) return false;
    size_t *older = calloc(m + 1, sizeof(*older));
    size_t *previous = malloc((m + 1) * sizeof(*previous));
    size_t *current = malloc((m + 1) * sizeof(*current));
    if (!older || !previous || !current) {
        free(older); free(previous); free(current);
        return false;
    }
    for (size_t j = 0; j <= m; ++j) previous[j] = j;
    for (size_t i = 1; i <= n; ++i) {
        current[0] = i;
        for (size_t j = 1; j <= m; ++j) {
            const size_t cost = a[i - 1] == b[j - 1] ? 0 : 1;
            size_t best = minimum(minimum(previous[j] + 1, current[j - 1] + 1),
                                  previous[j - 1] + cost);
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] &&
                a[i - 2] == b[j - 1])
                best = minimum(best, older[j - 2] + 1);
            current[j] = best;
        }
        size_t *tmp = older; older = previous; previous = current; current = tmp;
    }
    *result = previous[m];
    free(older); free(previous); free(current);
    return true;
}
