#include "median_absolute_error.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static int compare_doubles(const void *a, const void *b) {
    const double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

bool median_absolute_error(const double *actual, const double *predicted,
                           size_t n, double *result) {
    if (!actual || !predicted || !result || n == 0 ||
        n > SIZE_MAX / sizeof(double)) return false;
    double *errors = malloc(n * sizeof(*errors));
    if (!errors) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(actual[i]) || !isfinite(predicted[i])) {
            free(errors);
            return false;
        }
        errors[i] = fabs(actual[i] - predicted[i]);
        if (!isfinite(errors[i])) {
            free(errors);
            return false;
        }
    }
    qsort(errors, n, sizeof(*errors), compare_doubles);
    double score = errors[n / 2];
    if (n % 2 == 0) {
        const double lower = errors[n / 2 - 1];
        score = lower + (score - lower) / 2.0;
    }
    free(errors);
    *result = score;
    return true;
}
