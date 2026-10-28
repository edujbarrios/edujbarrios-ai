#include "gini_impurity.h"
#include <math.h>

bool gini_impurity(const uint32_t *counts, size_t n, double *out) {
    if (!counts || !out || n == 0 || n > 128) return false;
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (counts[i] > 1000000U) return false;
        total += (double)counts[i];
    }
    if (total == 0.0) return false;
    double sum_squares = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double p = (double)counts[i] / total;
        sum_squares += p*p;
    }
    *out = fmax(0.0, fmin(1.0, 1.0 - sum_squares));
    return true;
}
