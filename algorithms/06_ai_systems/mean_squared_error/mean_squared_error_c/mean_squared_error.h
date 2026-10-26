#ifndef EDUJBARRIOS_AI_MEAN_SQUARED_ERROR_H
#define EDUJBARRIOS_AI_MEAN_SQUARED_ERROR_H
#include <stdbool.h>
#include <stddef.h>

/* Nonempty finite real arrays of length n. Output unchanged on failure. */
bool mean_squared_error(const double *actual, const double *predicted,
                        size_t n, double *result);
#endif
