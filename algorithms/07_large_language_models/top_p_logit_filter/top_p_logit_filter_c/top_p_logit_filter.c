#include "top_p_logit_filter.h"
#include <math.h>

bool top_p_logit_filter(const double *logits, size_t vocab, double top_p,
                        bool *keep) {
    if (!logits || !keep || vocab < 1 || vocab > 4096 ||
        !isfinite(top_p) || top_p <= 0.0 || top_p > 1.0) return false;
    double maximum = -INFINITY;
    for (size_t i = 0; i < vocab; ++i) {
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000.0) return false;
        if (logits[i] > maximum) maximum = logits[i];
    }
    double weights[4096], total = 0.0;
    size_t indices[4096];
    for (size_t i = 0; i < vocab; ++i) {
        weights[i] = exp(logits[i] - maximum);
        total += weights[i];
        indices[i] = i;
    }
    /* Selection-sort ensures score-descending and ID-ascending order. */
    for (size_t i = 0; i < vocab; ++i) {
        size_t best = i;
        for (size_t j = i + 1; j < vocab; ++j)
            if (logits[indices[j]] > logits[indices[best]] ||
                (logits[indices[j]] == logits[indices[best]] &&
                 indices[j] < indices[best])) best = j;
        size_t tmp = indices[i];
        indices[i] = indices[best];
        indices[best] = tmp;
    }
    for (size_t i = 0; i < vocab; ++i) keep[i] = false;
    double cumulative = 0.0;
    for (size_t i = 0; i < vocab; ++i) {
        const size_t id = indices[i];
        keep[id] = true;
        cumulative += weights[id] / total;
        if (top_p < 1.0 && cumulative >= top_p) break;
    }
    return true;
}
