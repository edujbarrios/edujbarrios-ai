#include "weighted_jaccard_similarity.h"
#include <math.h>

bool weighted_jaccard_similarity(const double *a, const double *b,
                                 size_t n, double *result) {
    if (!a || !b || !result || n == 0) return false;
    double scale = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(a[i]) || !isfinite(b[i]) || a[i] < 0 || b[i] < 0)
            return false;
        scale = fmax(scale, fmax(a[i], b[i]));
    }
    if (scale == 0.0) {
        *result = 1.0;
        return true;
    }
    double numerator = 0.0, denominator = 0.0;
    for (size_t i = 0; i < n; ++i) {
        numerator += fmin(a[i], b[i]) / scale;
        denominator += fmax(a[i], b[i]) / scale;
    }
    const double score = numerator / denominator;
    if (!isfinite(score)) return false;
    *result = fmin(1.0, fmax(0.0, score));
    return true;
}
