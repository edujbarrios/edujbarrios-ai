#include "weighted_median.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
    double value;
    double weight;
} weighted_pair;

static int compare_pairs(const void *a, const void *b) {
    const double x = ((const weighted_pair *)a)->value;
    const double y = ((const weighted_pair *)b)->value;
    return (x > y) - (x < y);
}

bool weighted_median(const double *values, const double *weights,
                     size_t n, double *median_out) {
    if (!values || !weights || !median_out || n == 0 || n > 512)
        return false;
    weighted_pair pairs[512];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i]) || !isfinite(weights[i]) || weights[i] < 0.0)
            return false;
        pairs[i].value = values[i];
        pairs[i].weight = weights[i];
    }
    qsort(pairs, n, sizeof(pairs[0]), compare_pairs);
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        total += pairs[i].weight;
        if (!isfinite(total)) return false;
    }
    if (total <= 0.0) return false;
    double cumulative = 0.0;
    for (size_t i = 0; i < n; ++i) {
        cumulative += pairs[i].weight;
        if (cumulative >= total - cumulative) {
            *median_out = pairs[i].value;
            return true;
        }
    }
    return false;
}
