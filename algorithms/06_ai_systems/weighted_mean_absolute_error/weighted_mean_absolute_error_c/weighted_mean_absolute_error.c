#include "weighted_mean_absolute_error.h"
#include <math.h>
bool weighted_mean_absolute_error(const double *targets, const double *predictions,
                                  const double *weights, size_t n, double *result) {
    if (!targets || !predictions || !weights || !result || n == 0 || n > 4096) return false;
    double weighted_sum = 0.0, total_weight = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double y = targets[i], p = predictions[i], w = weights[i];
        if (!isfinite(y) || !isfinite(p) || !isfinite(w) ||
            fabs(y) > 1000000.0 || fabs(p) > 1000000.0 || w < 0.0 || w > 1000000.0) return false;
        weighted_sum += w * fabs(y - p);
        total_weight += w;
    }
    if (total_weight <= 0.0) return false;
    *result = weighted_sum / total_weight;
    return true;
}
