#include "trimmed_mean.h"
#include <math.h>
#include <stdlib.h>

static int compare_doubles(const void *lhs, const void *rhs) {
    const double a = *(const double *)lhs, b = *(const double *)rhs;
    return (a > b) - (a < b);
}

bool trimmed_mean(const double *values, size_t n, double trim_fraction,
                  double *mean_out) {
    if (!values || !mean_out || n == 0 || n > 512 ||
        !isfinite(trim_fraction) || trim_fraction < 0.0 ||
        trim_fraction >= 0.5) return false;

    double sorted[512];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
        sorted[i] = values[i];
    }
    qsort(sorted, n, sizeof(sorted[0]), compare_doubles);
    const size_t k = (size_t)floor((double)n * trim_fraction);
    const size_t retained = n - 2 * k;
    double result = 0.0;
    for (size_t i = k; i < n - k; ++i) {
        result += sorted[i] / (double)retained;
        if (!isfinite(result)) return false;
    }
    *mean_out = result;
    return true;
}
