#ifndef EDUJBARRIOS_AI_LOG_SOFTMAX_H
#define EDUJBARRIOS_AI_LOG_SOFTMAX_H
#include <stdbool.h>
#include <stddef.h>

/* Stable log-softmax of n finite logits, n in 1..10000.
 * Caller-owned output length n remains unchanged on error;
 * output and input must not overlap. */
bool log_softmax(const double *logits, size_t n, double *out);
#endif
