#include "log_cosh_loss.h"
#include <math.h>

bool log_cosh_loss(const double *target, const double *prediction,
                   size_t n, double *out) {
    if (!target || !prediction || !out || n < 1 || n > 10000) return false;
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double t = target[i], p = prediction[i];
        if (!isfinite(t) || !isfinite(p) ||
            fabs(t) > 1000000.0 || fabs(p) > 1000000.0) return false;
        const double a = fabs(p - t);
        double term = a + log1p(exp(-2.0 * a)) - log(2.0);
        sum += fmax(0.0, term);
    }
    double result = sum / (double)n;
    if (!isfinite(result)) return false;
    *out = result;
    return true;
}
