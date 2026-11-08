#ifndef EDUJBARRIOS_AI_WEIGHTED_MEDIAN_H
#define EDUJBARRIOS_AI_WEIGHTED_MEDIAN_H
#include <stdbool.h>
#include <stddef.h>
/* Lower weighted median of 1..512 finite values and finite nonnegative
   weights with a strictly positive finite total. Leaves output unchanged
   on invalid input. */
bool weighted_median(const double *values, const double *weights,
                     size_t n, double *median_out);
#endif
