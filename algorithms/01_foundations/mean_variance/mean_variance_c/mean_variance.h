#ifndef EDUJBARRIOS_AI_MEAN_VARIANCE_H
#define EDUJBARRIOS_AI_MEAN_VARIANCE_H

#include <stdbool.h>
#include <stddef.h>

/* Population variance, divisor n. Requires n > 0, a valid n-element
   input, and distinct non-null output pointers. Outputs unchanged on error. */
bool mean_variance(const double *values, size_t n,
                   double *mean_out, double *variance_out);

#endif
