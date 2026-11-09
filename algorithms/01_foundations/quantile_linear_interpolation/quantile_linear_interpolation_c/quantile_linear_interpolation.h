#ifndef EDUJBARRIOS_AI_QUANTILE_LINEAR_INTERPOLATION_H
#define EDUJBARRIOS_AI_QUANTILE_LINEAR_INTERPOLATION_H
#include <stdbool.h>
#include <stddef.h>
/* Type-7 sample quantile of 1..512 finite numbers and probability [0,1].
   Failure preserves the output value. */
bool quantile_linear_interpolation(
    const double *values, size_t n, double probability, double *quantile_out);
#endif
