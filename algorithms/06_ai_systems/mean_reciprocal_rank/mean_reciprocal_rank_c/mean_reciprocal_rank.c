#include "mean_reciprocal_rank.h"
#include <math.h>

bool mean_reciprocal_rank(const size_t *ranks, size_t n, double *out) {
    if (!ranks || !out || n == 0) return false;
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (ranks[i] != 0) {
            total += 1.0 / (double)ranks[i];
            if (!isfinite(total)) return false;
        }
    }
    const double score = total / (double)n;
    if (!isfinite(score) || score < 0.0 || score > 1.0) return false;
    *out = score;
    return true;
}
