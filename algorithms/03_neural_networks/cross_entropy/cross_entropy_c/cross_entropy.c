#include "cross_entropy.h"
#include <math.h>

bool cross_entropy(const double *logits, size_t n, size_t target, double *out) {
    if (!logits || !out || n == 0 || target >= n) return false;
    double maximum = logits[0];
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(logits[i])) return false;
        if (logits[i] > maximum) maximum = logits[i];
    }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        total += exp(logits[i] - maximum);
        if (!isfinite(total)) return false;
    }
    if (total <= 0.0) return false;
    const double delta = logits[target] - maximum;
    const double loss = log(total) - delta;
    if (!isfinite(delta) || !isfinite(loss)) return false;
    *out = fmax(0.0, loss);
    return true;
}
