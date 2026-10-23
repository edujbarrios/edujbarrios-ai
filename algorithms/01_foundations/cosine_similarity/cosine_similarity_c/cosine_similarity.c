#include "cosine_similarity.h"

#include <math.h>

bool cosine_similarity(const double *left, const double *right,
                       size_t n, double *out) {
    if (left == NULL || right == NULL || out == NULL || n == 0) {
        return false;
    }
    double max_left = 0.0;
    double max_right = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(left[i]) || !isfinite(right[i])) {
            return false;
        }
        max_left = fmax(max_left, fabs(left[i]));
        max_right = fmax(max_right, fabs(right[i]));
    }
    if (max_left == 0.0 || max_right == 0.0) {
        return false;
    }
    double dot = 0.0;
    double left_sq = 0.0;
    double right_sq = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double a = left[i] / max_left;
        const double b = right[i] / max_right;
        dot += a * b;
        left_sq += a * a;
        right_sq += b * b;
    }
    const double denom = sqrt(left_sq) * sqrt(right_sq);
    if (!isfinite(denom) || denom <= 0.0) {
        return false;
    }
    const double value = dot / denom;
    if (!isfinite(value)) {
        return false;
    }
    *out = fmax(-1.0, fmin(1.0, value));
    return true;
}
