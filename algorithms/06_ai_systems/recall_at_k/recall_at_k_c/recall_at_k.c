#include "recall_at_k.h"

bool recall_at_k(const uint8_t *relevance, size_t n, size_t k,
                 size_t total_relevant, double *out) {
    if (!relevance || !out || n == 0 || k == 0 || k > n ||
        total_relevant == 0) return false;
    size_t observed = 0, top_relevant = 0;
    for (size_t i = 0; i < n; ++i) {
        if (relevance[i] > 1) return false;
        if (relevance[i] == 1) {
            ++observed;
            if (i < k) ++top_relevant;
        }
    }
    if (observed > total_relevant) return false;
    *out = (double)top_relevant / (double)total_relevant;
    return true;
}
