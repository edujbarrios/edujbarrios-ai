#include "inverted_dropout.h"
#include <math.h>

bool inverted_dropout(const double *activations, const uint8_t *mask,
                      size_t n, double probability, double *output) {
    if (!activations || !mask || !output || !isfinite(probability) ||
        probability < 0.0 || probability >= 1.0) return false;
    const double keep = 1.0 - probability;
    /* Validate every element before touching the caller's output. */
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(activations[i]) || mask[i] > 1 ||
            (mask[i] && !isfinite(activations[i] / keep))) return false;
    }
    for (size_t i = 0; i < n; ++i) {
        output[i] = mask[i] ? activations[i] / keep : 0.0;
    }
    return true;
}
