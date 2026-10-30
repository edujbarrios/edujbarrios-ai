#include "weighted_brier_score.h"
#include <math.h>
bool weighted_brier_score(const double *targets, const double *probabilities,
                          const double *weights, size_t n, double *score) {
    if (!targets || !probabilities || !weights || !score || n == 0 || n > 4096) return false;
    double weighted_error = 0.0, total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double y = targets[i], p = probabilities[i], w = weights[i];
        if (!isfinite(y) || !isfinite(p) || !isfinite(w) ||
            (y != 0.0 && y != 1.0) || p < 0.0 || p > 1.0 ||
            w < 0.0 || w > 1000000.0) return false;
        double diff = p - y;
        weighted_error += w * diff * diff;
        total += w;
    }
    if (total <= 0.0) return false;
    *score = weighted_error / total;
    return true;
}
