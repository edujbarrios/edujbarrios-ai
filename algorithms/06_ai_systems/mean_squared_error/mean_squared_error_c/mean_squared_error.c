#include "mean_squared_error.h"
#include <math.h>

bool mean_squared_error(const double *actual, const double *predicted,
                        size_t n, double *result) {
    if (!actual || !predicted || !result || n == 0) return false;
    double average = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(actual[i]) || !isfinite(predicted[i])) return false;
        const double difference = actual[i] - predicted[i];
        const double squared = difference * difference;
        if (!isfinite(squared)) return false;
        average += (squared - average) / (double)(i + 1);
    }
    if (!isfinite(average)) return false;
    *result = fmax(0.0, average);
    return true;
}
