#ifndef EDUJBARRIOS_AI_MEAN_ABSOLUTE_ERROR_H
#define EDUJBARRIOS_AI_MEAN_ABSOLUTE_ERROR_H
#include <stdbool.h>
#include <stddef.h>
/* Unweighted MAE for n>=1 finite samples. On failure *out stays unchanged.
   Caller guarantees arrays of >=n values and no alias with out. */
bool mean_absolute_error(const double *truth, const double *predictions,
                         size_t n, double *out);
#endif
