#include "overlap_coefficient.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int compare_ids(const void *lhs, const void *rhs) {
    const int32_t a = *(const int32_t *)lhs, b = *(const int32_t *)rhs;
    return (a > b) - (a < b);
}

static size_t make_unique(int32_t *items, size_t count) {
    if (count == 0) return 0;
    qsort(items, count, sizeof(*items), compare_ids);
    size_t unique = 1;
    for (size_t i = 1; i < count; ++i)
        if (items[i] != items[unique - 1]) items[unique++] = items[i];
    return unique;
}

bool overlap_coefficient(const int32_t *a, size_t n, const int32_t *b,
                         size_t m, double *result) {
    if (!result || (n && !a) || (m && !b) ||
        n > SIZE_MAX / sizeof(int32_t) ||
        m > SIZE_MAX / sizeof(int32_t)) return false;
    int32_t *acopy = NULL, *bcopy = NULL;
    if (n) {
        acopy = malloc(n * sizeof(*acopy));
        if (!acopy) return false;
        memcpy(acopy, a, n * sizeof(*acopy));
    }
    if (m) {
        bcopy = malloc(m * sizeof(*bcopy));
        if (!bcopy) {free(acopy); return false;}
        memcpy(bcopy, b, m * sizeof(*bcopy));
    }
    const size_t na = make_unique(acopy, n);
    const size_t nb = make_unique(bcopy, m);
    size_t i = 0, j = 0, common = 0;
    while (i < na && j < nb) {
        if (acopy[i] == bcopy[j]) { ++common; ++i; ++j; }
        else if (acopy[i] < bcopy[j]) ++i;
        else ++j;
    }
    double answer;
    if (na == 0 && nb == 0) answer = 1.0;
    else if (na == 0 || nb == 0) answer = 0.0;
    else answer = (double)common / (double)(na < nb ? na : nb);
    free(acopy);
    free(bcopy);
    *result = answer;
    return true;
}
