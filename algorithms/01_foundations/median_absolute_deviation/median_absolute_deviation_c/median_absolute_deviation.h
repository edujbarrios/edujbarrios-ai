#ifndef EDUJBARRIOS_AI_MEDIAN_ABSOLUTE_DEVIATION_H
#define EDUJBARRIOS_AI_MEDIAN_ABSOLUTE_DEVIATION_H
#include <stdbool.h>
#include <stddef.h>
/* Raw unscaled MAD for 1..512 finite double values.
   On invalid input or nonfinite intermediate, mad_out is unchanged. */
bool median_absolute_deviation(const double *values, size_t n, double *mad_out);
#endif
