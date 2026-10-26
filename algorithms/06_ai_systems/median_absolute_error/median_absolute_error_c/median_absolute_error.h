#ifndef EDUJBARRIOS_AI_MEDIAN_ABSOLUTE_ERROR_H
#define EDUJBARRIOS_AI_MEDIAN_ABSOLUTE_ERROR_H
#include <stdbool.h>
#include <stddef.h>

/* Equal nonempty finite arrays of length n; no output change on failure. */
bool median_absolute_error(const double *actual, const double *predicted,
                           size_t n, double *result);
#endif
