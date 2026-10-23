#include "ndcg_at_k.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int compare_desc(const void *a, const void *b) {
    const uint8_t x = *(const uint8_t *)a;
    const uint8_t y = *(const uint8_t *)b;
    if (x > y) return -1;
    if (x < y) return 1;
    return 0;
}
static double dcg(const uint8_t *grades, size_t k) {
    double sum = 0.0;
    for (size_t i = 0; i < k; ++i) {
        const double gain = (double)((1u << grades[i]) - 1u);
        sum += gain / log2((double)i + 2.0);
    }
    return sum;
}
bool ndcg_at_k(const uint8_t *grades, size_t n, size_t k, double *out) {
    if (!grades || !out || n == 0 || k == 0 || k > n ||
        n > SIZE_MAX / sizeof(uint8_t)) return false;
    for (size_t i = 0; i < n; ++i) {
        if (grades[i] > 20) return false;
    }
    uint8_t *ideal = malloc(n * sizeof(*ideal));
    if (!ideal) return false;
    memcpy(ideal, grades, n);
    qsort(ideal, n, sizeof(*ideal), compare_desc);
    const double current = dcg(grades, k);
    const double maximum = dcg(ideal, k);
    free(ideal);
    if (!isfinite(current) || !isfinite(maximum) || maximum < 0.0) return false;
    const double result = maximum == 0.0 ? 0.0 : current / maximum;
    if (!isfinite(result) || result < 0.0) return false;
    *out = fmax(0.0, fmin(1.0, result));
    return true;
}
