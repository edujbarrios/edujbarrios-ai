#include "temperature_softmax.h"
#include <math.h>

bool temperature_softmax(const double *logits, size_t length,
                         double temperature, double *probabilities) {
    if (!logits || !probabilities || length < 1 || length > 4096 ||
        !isfinite(temperature) || temperature < 0.01 ||
        temperature > 100.0) return false;
    double maximum = -INFINITY;
    for (size_t i = 0; i < length; ++i) {
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000.0)
            return false;
        if (logits[i] > maximum) maximum = logits[i];
    }
    double total = 0.0;
    for (size_t i = 0; i < length; ++i) {
        probabilities[i] = exp((logits[i] - maximum) / temperature);
        total += probabilities[i];
    }
    for (size_t i = 0; i < length; ++i)
        probabilities[i] /= total;
    return true;
}
