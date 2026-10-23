#ifndef EDUJBARRIOS_AI_MIN_MAX_SCALING_H
#define EDUJBARRIOS_AI_MIN_MAX_SCALING_H
#include <stdbool.h>
#include <stddef.h>
/* Input/output buffers contain at least n doubles. Exact in-place scaling
   is supported. Output is unchanged on error; partial overlap unsupported. */
bool min_max_scaling(const double *input, size_t n, double *output);
#endif
