#ifndef EDUJBARRIOS_AI_CROSS_ENTROPY_H
#define EDUJBARRIOS_AI_CROSS_ENTROPY_H
#include <stdbool.h>
#include <stddef.h>

/* Finite logits of size n > 0, zero-based target < n.
   Output double pointer must be nonnull; unchanged on invalid input. */
bool cross_entropy(const double *logits, size_t n, size_t target, double *out);
#endif
