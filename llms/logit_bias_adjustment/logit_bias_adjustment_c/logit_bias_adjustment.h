#ifndef LOGIT_BIAS_ADJUSTMENT_H
#define LOGIT_BIAS_ADJUSTMENT_H
#include <stdbool.h>
#include <stddef.h>
/* n logits, b pairs of zero-based token IDs and biases.
   On error output is unchanged; exact in-place operation is allowed. */
bool logit_bias_adjustment(const double *logits, size_t n,
                           const size_t *token_ids, const double *biases,
                           size_t b, double *output);
#endif
