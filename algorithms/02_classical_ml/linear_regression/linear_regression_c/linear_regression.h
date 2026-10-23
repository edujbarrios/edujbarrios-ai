#ifndef EDUJBARRIOS_AI_LINEAR_REGRESSION_H
#define EDUJBARRIOS_AI_LINEAR_REGRESSION_H
#include <stdbool.h>
#include <stddef.h>

/* Caller provides n paired samples in x and y; n >= 2.
   Outputs are distinct non-null pointers, unchanged on failure. */
bool linear_regression(const double *x, const double *y, size_t n,
                       double *slope, double *intercept);
#endif
