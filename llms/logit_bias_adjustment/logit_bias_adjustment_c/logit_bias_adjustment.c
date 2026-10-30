#include "logit_bias_adjustment.h"
#include <math.h>
#include <string.h>
bool logit_bias_adjustment(const double *logits, size_t n,
                           const size_t *token_ids, const double *biases,
                           size_t b, double *output) {
    if (!logits || !output || n == 0 || n > 4096 || b > n ||
        (b > 0 && (!token_ids || !biases))) return false;
    for (size_t i = 0; i < n; ++i)
        if (!isfinite(logits[i]) || fabs(logits[i]) > 1000.0) return false;
    unsigned char used[4096] = {0};
    for (size_t j = 0; j < b; ++j) {
        if (token_ids[j] >= n || used[token_ids[j]] ||
            !isfinite(biases[j]) || fabs(biases[j]) > 100.0) return false;
        used[token_ids[j]] = 1;
    }
    /* memmove handles exact input/output alias; partially overlapping buffers
       are also safe for the initial copy, but not promised by the contract. */
    memmove(output, logits, n * sizeof(*output));
    for (size_t j = 0; j < b; ++j) output[token_ids[j]] += biases[j];
    return true;
}
