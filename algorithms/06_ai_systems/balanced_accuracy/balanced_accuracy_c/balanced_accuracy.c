#include "balanced_accuracy.h"
#include <stddef.h>
#include <stdint.h>

bool balanced_accuracy(const uint8_t *truth, const uint8_t *predictions,
                       size_t n, double *out) {
    if (truth == NULL || predictions == NULL || out == NULL || n == 0) {
        return false;
    }
    size_t positives = 0, negatives = 0, true_positive = 0, true_negative = 0;
    for (size_t i = 0; i < n; ++i) {
        const uint8_t y = truth[i], p = predictions[i];
        if (y > 1 || p > 1) return false;
        if (y == 1) {
            ++positives;
            if (p == 1) ++true_positive;
        } else {
            ++negatives;
            if (p == 0) ++true_negative;
        }
    }
    if (positives == 0 || negatives == 0) return false;
    *out = 0.5 * ((double)true_positive / (double)positives +
                  (double)true_negative / (double)negatives);
    return true;
}
