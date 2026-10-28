#include "log_softmax.h"
#include <math.h>
#include <string.h>

bool log_softmax(const double *logits, size_t n, double *out) {
    if (!logits || !out || n < 1 || n > 10000) return false;
    double largest = -INFINITY;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000000.0) return false;
        if (logits[i] > largest) largest = logits[i];
    }
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i)
        sum += exp(logits[i] - largest);
    if (!isfinite(sum) || sum <= 0.0) return false;
    const double correction = log(sum);
    double result[10000];
    for (size_t i = 0; i < n; ++i) {
        result[i] = (logits[i] - largest) - correction;
        if (!isfinite(result[i])) return false;
    }
    memcpy(out, result, n * sizeof(*out));
    return true;
}
