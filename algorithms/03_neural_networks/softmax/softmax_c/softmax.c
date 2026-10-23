#include "softmax.h"
#include <math.h>

bool softmax(const double *logits, size_t n, double *output) {
    if (!logits || !output || n == 0) return false;
    double maximum = logits[0];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(logits[i])) return false;
        if (logits[i] > maximum) maximum = logits[i];
    }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        /* logits[i] - maximum <= 0 mathematically; both finite. */
        total += exp(logits[i] - maximum);
        if (!isfinite(total)) return false;
    }
    if (total <= 0.0) return false;
    for (size_t i = 0; i < n; ++i) {
        output[i] = exp(logits[i] - maximum) / total;
    }
    return true;
}
