#include "gaussian_kde_1d.h"
#include <math.h>

bool gaussian_kde_1d(const double *samples, size_t n, double query,
                     double bandwidth, double *output) {
    if (!samples || !output || n == 0 || n > 512 ||
        !isfinite(query) || fabs(query) > 1000.0 ||
        !isfinite(bandwidth) || bandwidth < 0.001 || bandwidth > 1000.0)
        return false;
    for (size_t i = 0; i < n; ++i)
        if (!isfinite(samples[i]) || fabs(samples[i]) > 1000.0)
            return false;

    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double z = (query - samples[i]) / bandwidth;
        total += exp(-0.5 * z * z);
    }
    const double sqrt_2_pi = 2.5066282746310005024;
    *output = total / ((double)n * bandwidth * sqrt_2_pi);
    return true;
}
