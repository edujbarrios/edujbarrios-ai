#ifndef EDUJBARRIOS_AI_RELU_H
#define EDUJBARRIOS_AI_RELU_H
#include <stdbool.h>
#include <stddef.h>

/* Caller guarantees buffers of n doubles; exact in-place output allowed.
   Inputs may be NULL when n == 0; output pointer must be non-null.
   On invalid input output is unchanged. */
bool relu(const double *values, size_t n, double *output);
#endif
