#include "greedy_token_selection.h"
#include <math.h>

bool greedy_token_selection(const double *logits, size_t n, size_t *index) {
    if (!logits || !index || n == 0) return false;
    double maximum = logits[0];
    size_t best = 0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(logits[i])) return false;
        if (logits[i] > maximum) {
            maximum = logits[i];
            best = i;
        }
    }
    *index = best;
    return true;
}
