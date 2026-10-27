#include "min_p_logit_filter.h"
#include <math.h>

bool min_p_logit_filter(const double *logits, size_t vocab,
                        double min_p, bool *keep) {
    if (!logits || !keep || vocab < 1 || vocab > 4096 ||
        !isfinite(min_p) || min_p < 0.0 || min_p > 1.0)
        return false;
    double maximum = -INFINITY;
    for (size_t i = 0; i < vocab; ++i) {
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000.0)
            return false;
        if (logits[i] > maximum) maximum = logits[i];
    }
    for (size_t i = 0; i < vocab; ++i)
        keep[i] = min_p == 0.0 || exp(logits[i] - maximum) >= min_p;
    return true;
}
