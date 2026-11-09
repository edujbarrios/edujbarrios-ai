#ifndef EDUJBARRIOS_AI_WINSORIZED_MEAN_H
#define EDUJBARRIOS_AI_WINSORIZED_MEAN_H
#include <stdbool.h>
#include <stddef.h>
/* Symmetric winsorized mean of 1..512 finite numbers; fraction in [0,0.5).
   Output is unchanged on validation failure or nonfinite intermediate. */
bool winsorized_mean(const double *values, size_t n, double trim_fraction,
                     double *mean_out);
#endif
