#include "total_variation_distance.h"
#include <math.h>

bool total_variation_distance(const double *p, const double *q,
                              size_t n, double *result) {
    if (!p || !q || !result || n == 0) return false;
    double p_sum = 0.0, q_sum = 0.0, delta_sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(p[i]) || !isfinite(q[i]) ||
            p[i] < 0.0 || q[i] < 0.0 || p[i] > 1.0 || q[i] > 1.0) {
            return false;
        }
        p_sum += p[i];
        q_sum += q[i];
        delta_sum += fabs(p[i] - q[i]);
    }
    if (fabs(p_sum - 1.0) > 1e-9 || fabs(q_sum - 1.0) > 1e-9) return false;
    *result = fmax(0.0, fmin(1.0, 0.5 * delta_sum));
    return true;
}
