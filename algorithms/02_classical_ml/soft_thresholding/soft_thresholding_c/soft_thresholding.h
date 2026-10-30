#ifndef SOFT_THRESHOLDING_H
#define SOFT_THRESHOLDING_H
#include <stdbool.h>
#include <stddef.h>
/* Exact in-place operation supported. On error output remains unchanged. */
bool soft_thresholding(const double *values, size_t n, double threshold, double *output);
#endif
