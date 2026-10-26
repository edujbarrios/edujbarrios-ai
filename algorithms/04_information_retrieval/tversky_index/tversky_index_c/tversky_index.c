#include "tversky_index.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int compare_ids(const void *a, const void *b) {
    const int32_t x = *(const int32_t *)a, y = *(const int32_t *)b;
    return (x > y) - (x < y);
}

static size_t unique_ids(int32_t *a, size_t n) {
    if (n == 0) return 0;
    qsort(a, n, sizeof(*a), compare_ids);
    size_t length = 1;
    for (size_t i = 1; i < n; ++i)
        if (a[i] != a[length - 1]) a[length++] = a[i];
    return length;
}

bool tversky_index(const int32_t *a, size_t n, const int32_t *b, size_t m,
                   double alpha, double beta, double *result) {
    if (!result || (n && !a) || (m && !b) ||
        !isfinite(alpha) || !isfinite(beta) || alpha <= 0 || beta <= 0 ||
        n > SIZE_MAX / sizeof(int32_t) || m > SIZE_MAX / sizeof(int32_t))
        return false;
    int32_t *aa = NULL, *bb = NULL;
    if (n) {
        aa = malloc(n * sizeof(*aa));
        if (!aa) return false;
        memcpy(aa, a, n * sizeof(*aa));
    }
    if (m) {
        bb = malloc(m * sizeof(*bb));
        if (!bb) { free(aa); return false; }
        memcpy(bb, b, m * sizeof(*bb));
    }
    const size_t na = unique_ids(aa, n), nb = unique_ids(bb, m);
    size_t i = 0, j = 0, common = 0;
    while (i < na && j < nb) {
        if (aa[i] == bb[j]) {++i; ++j; ++common;}
        else if (aa[i] < bb[j]) ++i;
        else ++j;
    }
    double answer = 0.0;
    if (na == 0 && nb == 0) answer = 1.0;
    else if (na && nb) {
        const double denom = (double)common +
            alpha * (double)(na - common) + beta * (double)(nb - common);
        if (!isfinite(denom) || denom <= 0) {
            free(aa); free(bb); return false;
        }
        answer = (double)common / denom;
    }
    free(aa);
    free(bb);
    *result = fmin(1.0, fmax(0.0, answer));
    return true;
}
