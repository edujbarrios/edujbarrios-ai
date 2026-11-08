#include "median_absolute_deviation.h"
#include <math.h>
#include <stdlib.h>

static int compare_doubles(const void *a, const void *b) {
    const double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

static double middle(const double *sorted, size_t n) {
    if (n % 2 != 0) return sorted[n / 2];
    const double left = sorted[n / 2 - 1], right = sorted[n / 2];
    if (left <= 0.0 && right >= 0.0) return left / 2.0 + right / 2.0;
    return left + (right - left) / 2.0;
}

bool median_absolute_deviation(const double *values, size_t n, double *mad_out) {
    if (!values || !mad_out || n == 0 || n > 512) return false;
    double sorted[512], deviations[512];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
        sorted[i] = values[i];
    }
    qsort(sorted, n, sizeof(double), compare_doubles);
    const double median = middle(sorted, n);
    if (!isfinite(median)) return false;
    for (size_t i = 0; i < n; ++i) {
        deviations[i] = fabs(values[i] - median);
        if (!isfinite(deviations[i])) return false;
    }
    qsort(deviations, n, sizeof(double), compare_doubles);
    const double result = middle(deviations, n);
    if (!isfinite(result) || result < 0.0) return false;
    *mad_out = result;
    return true;
}
