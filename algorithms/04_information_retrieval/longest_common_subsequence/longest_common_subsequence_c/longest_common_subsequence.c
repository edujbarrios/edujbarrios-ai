#include "longest_common_subsequence.h"
#include <stdint.h>
#include <stdlib.h>

bool longest_common_subsequence(const int32_t *a, size_t n,
                                const int32_t *b, size_t m, size_t *result) {
    if (!result || (n && !a) || (m && !b)) return false;
    if (n == 0 || m == 0) {
        *result = 0;
        return true;
    }
    if (m > n) {
        const int32_t *tmp = a; a = b; b = tmp;
        const size_t len = n; n = m; m = len;
    }
    if (m >= SIZE_MAX / sizeof(size_t)) return false;
    size_t *previous = calloc(m + 1, sizeof(*previous));
    size_t *current = calloc(m + 1, sizeof(*current));
    if (!previous || !current) {
        free(previous); free(current); return false;
    }
    for (size_t i = 0; i < n; ++i) {
        current[0] = 0;
        for (size_t j = 1; j <= m; ++j) {
            if (a[i] == b[j-1]) {
                current[j] = previous[j-1] + 1;
            } else {
                current[j] = previous[j] > current[j-1]
                    ? previous[j] : current[j-1];
            }
        }
        size_t *tmp = previous; previous = current; current = tmp;
    }
    *result = previous[m];
    free(previous);
    free(current);
    return true;
}
