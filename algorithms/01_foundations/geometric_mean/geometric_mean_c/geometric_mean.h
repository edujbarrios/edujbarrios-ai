#ifndef EDUJBARRIOS_AI_GEOMETRIC_MEAN_H
#define EDUJBARRIOS_AI_GEOMETRIC_MEAN_H
#include <stdbool.h>
#include <stddef.h>
/* Geometric mean of 1..512 finite nonnegative doubles.
   On failure output is unchanged. */
bool geometric_mean(const double *values, size_t n, double *mean_out);
#endif
