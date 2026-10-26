#include "mean_absolute_error.h"
#include <math.h>
#include <stddef.h>

bool mean_absolute_error(const double *truth, const double *predictions,
                         size_t n, double *out) {
    if (n == 0 || truth == NULL || predictions == NULL || out == NULL) {
        return false;
    }
    double average = 0.0;
    const double count = (double)n;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(truth[i]) || !isfinite(predictions[i])) return false;
        const double residual = truth[i] - predictions[i];
        if (!isfinite(residual)) return false;
        average += fabs(residual) / count;
        if (!isfinite(average)) return false;
    }
    *out = average;
    return true;
}
