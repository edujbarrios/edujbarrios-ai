#include "precision_at_k.h"

bool precision_at_k(const uint8_t *relevance, size_t n, size_t k, double *out) {
    if (!relevance || !out || n == 0 || k == 0 || k > n) return false;
    size_t relevant = 0;
    for (size_t i = 0; i < n; ++i) {
        if (relevance[i] > 1) return false;
        if (i < k && relevance[i] == 1) ++relevant;
    }
    *out = (double)relevant / (double)k;
    return true;
}
