#include "repetition_penalty.h"
#include <math.h>

bool repetition_penalty(const double *logits, size_t vocab,
                        const size_t *history, size_t length,
                        double penalty, double *out) {
    if (!logits || !out || (length && !history) || vocab < 1 ||
        vocab > 4096 || length > 16384 ||
        !isfinite(penalty) || penalty < 1.0 || penalty > 10.0)
        return false;
    for (size_t i = 0; i < vocab; ++i)
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000.0) return false;
    bool seen[4096] = {false};
    for (size_t i = 0; i < length; ++i) {
        if (history[i] >= vocab) return false;
        seen[history[i]] = true;
    }
    for (size_t i = 0; i < vocab; ++i) {
        const double x = logits[i];
        out[i] = seen[i] ? (x > 0.0 ? x / penalty : x * penalty) : x;
    }
    return true;
}
