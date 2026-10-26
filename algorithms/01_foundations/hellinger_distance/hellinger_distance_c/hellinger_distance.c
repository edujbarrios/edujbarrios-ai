#include "hellinger_distance.h"
#include <math.h>

bool hellinger_distance(const double *p, const double *q, size_t n, double *result) {
    if (!p || !q || !result || n == 0) return false;
    double sp = 0.0, sq = 0.0, squared = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(p[i]) || !isfinite(q[i]) ||
            p[i] < 0.0 || p[i] > 1.0 || q[i] < 0.0 || q[i] > 1.0)
            return false;
        sp += p[i];
        sq += q[i];
        const double delta = sqrt(p[i]) - sqrt(q[i]);
        squared += delta * delta;
    }
    if (fabs(sp - 1.0) > 1e-9 || fabs(sq - 1.0) > 1e-9) return false;
    const double distance = sqrt(squared / 2.0);
    *result = fmin(1.0, fmax(0.0, distance));
    return true;
}
