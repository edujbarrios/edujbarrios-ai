#ifndef EDUJBARRIOS_AI_SWISH_H
#define EDUJBARRIOS_AI_SWISH_H
#include <stdbool.h>
#include <stddef.h>
/* x[n] -> values[n], derivatives[n]; all buffers distinct and correctly
   sized. No output changes on invalid inputs or allocation failure. */
bool swish(const double *x, size_t n, double *values, double *derivatives);
#endif
