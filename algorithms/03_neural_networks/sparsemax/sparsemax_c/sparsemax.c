#include "sparsemax.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static int descending(const void *left, const void *right) {
    double a = *(const double *)left;
    double b = *(const double *)right;
    return (a < b) - (a > b);
}

bool sparsemax(const double *logits, size_t n, double *output) {
    if (!logits || !output || n == 0 || n > SIZE_MAX / sizeof(double)) {
        return false;
    }
    double maximum = logits[0];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(logits[i])) return false;
        if (logits[i] > maximum) maximum = logits[i];
    }
    double *sorted = malloc(n * sizeof(*sorted));
    if (!sorted) return false;
    for (size_t i = 0; i < n; ++i) sorted[i] = logits[i] - maximum;
    qsort(sorted, n, sizeof(*sorted), descending);

    double prefix = 0.0;
    double threshold = 0.0;
    size_t support = 0;
    for (size_t i = 0; i < n; ++i) {
        prefix += sorted[i];
        if (1.0 + (double)(i + 1) * sorted[i] > prefix) {
            support = i + 1;
            threshold = (prefix - 1.0) / (double)support;
        }
    }
    free(sorted);
    /* The largest shifted logit is zero, so support is always nonempty. */
    if (support == 0) return false;
    for (size_t i = 0; i < n; ++i) {
        double projected = (logits[i] - maximum) - threshold;
        output[i] = projected > 0.0 ? projected : 0.0;
    }
    return true;
}
