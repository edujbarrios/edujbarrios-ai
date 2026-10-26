#ifndef EDUJBARRIOS_AI_BRAY_CURTIS_DISSIMILARITY_H
#define EDUJBARRIOS_AI_BRAY_CURTIS_DISSIMILARITY_H

#include <stdbool.h>
#include <stddef.h>

/* Nonempty aligned nonnegative finite arrays; result unchanged on error. */
bool bray_curtis_dissimilarity(const double *a, const double *b,
                              size_t n, double *result);
#endif
