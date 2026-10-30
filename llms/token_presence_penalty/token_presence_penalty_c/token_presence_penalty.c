#include "token_presence_penalty.h"
#include <math.h>
bool token_presence_penalty(const double *logits, size_t vocab_size,
                            const size_t *history, size_t history_length,
                            double penalty, double *output) {
    if (!logits || !output || vocab_size == 0 || vocab_size > 4096 ||
        history_length > 4096 || (history_length > 0 && !history) ||
        !isfinite(penalty) || penalty < 0 || penalty > 5) return false;
    for (size_t i = 0; i < vocab_size; ++i) {
        if (!isfinite(logits[i]) || logits[i] < -1000 || logits[i] > 1000) return false;
    }
    unsigned char present[4096] = {0};
    for (size_t i = 0; i < history_length; ++i) {
        if (history[i] >= vocab_size) return false;
        present[history[i]] = 1;
    }
    for (size_t i = 0; i < vocab_size; ++i) {
        output[i] = logits[i] - (present[i] ? penalty : 0.0);
    }
    return true;
}
