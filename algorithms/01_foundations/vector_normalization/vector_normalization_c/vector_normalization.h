#ifndef EDUJBARRIOS_AI_VECTOR_NORMALIZATION_H
#define EDUJBARRIOS_AI_VECTOR_NORMALIZATION_H

#include <stdbool.h>
#include <stddef.h>

/* Normalize finite, nonzero left[0..n) into output[0..n).
   Caller supplies valid buffers; exact in-place normalization is supported.
   No output is written when validation fails. */
bool vector_normalization(const double *left, size_t n, double *output);

#endif
