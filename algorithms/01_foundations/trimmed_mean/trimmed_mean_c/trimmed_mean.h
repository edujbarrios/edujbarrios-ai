#ifndef EDUJBARRIOS_AI_TRIMMED_MEAN_H
#define EDUJBARRIOS_AI_TRIMMED_MEAN_H
#include <stdbool.h>
#include <stddef.h>
/* Symmetric trimmed mean for 1..512 finite values, fraction in [0,0.5).
   mean_out is unchanged on invalid input or nonfinite intermediate. */
bool trimmed_mean(const double *values, size_t n, double trim_fraction,
                  double *mean_out);
#endif
