#include "top_k_logit_filter.h"
#include <math.h>

bool top_k_logit_filter(const double *logits, size_t vocab, size_t k,
                        bool *keep) {
    if (!logits || !keep || vocab < 1 || vocab > 4096 ||
        k < 1 || k > vocab) return false;
    for (size_t i = 0; i < vocab; ++i)
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000.0)
            return false;
    for (size_t i = 0; i < vocab; ++i) {
        size_t rank = 0;
        for (size_t j = 0; j < vocab; ++j)
            if (logits[j] > logits[i] ||
                (logits[j] == logits[i] && j < i)) ++rank;
        keep[i] = rank < k;
    }
    return true;
}
