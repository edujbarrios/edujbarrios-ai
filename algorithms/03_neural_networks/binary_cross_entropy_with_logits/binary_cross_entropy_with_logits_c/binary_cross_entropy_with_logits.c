#include "binary_cross_entropy_with_logits.h"
#include <math.h>

bool binary_cross_entropy_with_logits(const double *logits,
    const double *targets, size_t n, double *out) {
    if (!logits || !targets || !out || n == 0 || n > 10000) return false;
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double z = logits[i], y = targets[i];
        if (!isfinite(z) || fabs(z) > 1000000.0 ||
            !isfinite(y) || y < 0.0 || y > 1.0) return false;
        double term = fmax(z, 0.0) - z*y + log1p(exp(-fabs(z)));
        total += fmax(0.0, term);
    }
    double mean = total / (double)n;
    if (!isfinite(mean)) return false;
    *out = mean;
    return true;
}
