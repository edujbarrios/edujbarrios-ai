#include "dice_similarity.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int compare_ids(const void *lhs, const void *rhs) {
    const int32_t a = *(const int32_t *)lhs, b = *(const int32_t *)rhs;
    return (a > b) - (a < b);
}

static size_t unique_sorted(int32_t *values, size_t n) {
    if (n == 0) return 0;
    qsort(values, n, sizeof(*values), compare_ids);
    size_t unique_count = 1;
    for (size_t i = 1; i < n; ++i) {
        if (values[i] != values[unique_count - 1])
            values[unique_count++] = values[i];
    }
    return unique_count;
}

bool dice_similarity(const int32_t *first, size_t n,
                     const int32_t *second, size_t m, double *result) {
    if (!result || (n && !first) || (m && !second) ||
        n > SIZE_MAX / sizeof(int32_t) ||
        m > SIZE_MAX / sizeof(int32_t)) return false;
    int32_t *a = NULL, *b = NULL;
    if (n) {
        a = malloc(n * sizeof(*a));
        if (!a) return false;
        memcpy(a, first, n * sizeof(*a));
    }
    if (m) {
        b = malloc(m * sizeof(*b));
        if (!b) { free(a); return false; }
        memcpy(b, second, m * sizeof(*b));
    }
    const size_t na = unique_sorted(a, n), nb = unique_sorted(b, m);
    size_t i = 0, j = 0, intersection = 0;
    while (i < na && j < nb) {
        if (a[i] == b[j]) { ++intersection; ++i; ++j; }
        else if (a[i] < b[j]) ++i;
        else ++j;
    }
    const double denominator = (double)na + (double)nb;
    const double score = denominator == 0.0
        ? 1.0 : (2.0 * (double)intersection) / denominator;
    free(a);
    free(b);
    *result = score;
    return true;
}
