#ifndef EDUJBARRIOS_AI_LEAKY_RELU_H
#define EDUJBARRIOS_AI_LEAKY_RELU_H
#include <stdbool.h>
#include <stddef.h>
/* Caller guarantees buffers of n doubles; in-place is supported.
   out must be non-null. On error out is unchanged. */
bool leaky_relu(const double *values, size_t n, double alpha, double *out);
#endif
