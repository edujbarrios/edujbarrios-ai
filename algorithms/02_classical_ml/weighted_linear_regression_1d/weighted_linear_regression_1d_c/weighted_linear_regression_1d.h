#ifndef EDUJBARRIOS_AI_WEIGHTED_LINEAR_REGRESSION_1D_H
#define EDUJBARRIOS_AI_WEIGHTED_LINEAR_REGRESSION_1D_H
#include <stdbool.h>
#include <stddef.h>

/* Weighted least squares with intercept, 2..128 observations.
 * Each output stays unchanged on failure. Output pointers must be
 * distinct and must not overlap inputs. */
bool weighted_linear_regression_1d(const double *x, const double *y,
    const double *weights, size_t n, double *slope, double *intercept);
#endif
