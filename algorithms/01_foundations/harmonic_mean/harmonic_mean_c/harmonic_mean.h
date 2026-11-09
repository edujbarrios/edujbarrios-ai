#ifndef EDUJBARRIOS_AI_HARMONIC_MEAN_H
#define EDUJBARRIOS_AI_HARMONIC_MEAN_H
#include <stdbool.h>
#include <stddef.h>
/* Harmonic mean of 1..512 finite nonnegative doubles. Zero => zero.
   On invalid inputs or nonfinite output, mean_out is unchanged. */
bool harmonic_mean(const double *values, size_t n, double *mean_out);
#endif
