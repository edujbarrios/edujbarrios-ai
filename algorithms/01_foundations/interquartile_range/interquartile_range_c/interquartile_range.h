#ifndef EDUJBARRIOS_AI_INTERQUARTILE_RANGE_H
#define EDUJBARRIOS_AI_INTERQUARTILE_RANGE_H
#include <stdbool.h>
#include <stddef.h>
/* Q3-Q1, type-7 quartile interpolation, for 1..512 finite observations.
   Return false without changing output on invalid/nonfinite results. */
bool interquartile_range(const double *values, size_t n, double *iqr_out);
#endif
