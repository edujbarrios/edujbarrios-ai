#ifndef WEIGHTED_MEAN_ABSOLUTE_ERROR_H
#define WEIGHTED_MEAN_ABSOLUTE_ERROR_H
#include <stdbool.h>
#include <stddef.h>
/* All buffers contain n values; *result remains unchanged on failure. */
bool weighted_mean_absolute_error(const double *targets, const double *predictions,
                                  const double *weights, size_t n, double *result);
#endif
