#include "winsorized_mean.h"
#include <math.h>
#include <stdlib.h>

static int compare_doubles(const void *a, const void *b) {
    const double x = *(const double *)a;
    const double y = *(const double *)b;
    return (x > y) - (x < y);
}

bool winsorized_mean(const double *values, size_t n, double trim_fraction,
                     double *mean_out) {
    if (!values || !mean_out || n < 1 || n > 512 ||
        !isfinite(trim_fraction) || trim_fraction < 0.0 || trim_fraction >= 0.5) {
        return false;
    }
    double sorted[512];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
        sorted[i] = values[i];
    }
    qsort(sorted, n, sizeof(double), compare_doubles);
    const size_t k = (size_t)floor((double)n * trim_fraction);
    const double low = sorted[k], high = sorted[n - k - 1];
    double result = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double clipped = i < k ? low : (i >= n - k ? high : sorted[i]);
        result += clipped / (double)n;
        if (!isfinite(result)) return false;
    }
    *mean_out = result;
    return true;
}
