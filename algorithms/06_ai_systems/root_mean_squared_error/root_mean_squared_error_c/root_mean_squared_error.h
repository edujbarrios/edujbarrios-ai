#ifndef EDUJBARRIOS_AI_ROOT_MEAN_SQUARED_ERROR_H
#define EDUJBARRIOS_AI_ROOT_MEAN_SQUARED_ERROR_H
#include <stdbool.h>
#include <stddef.h>

/* Scalar RMSE over paired bounded finite vectors (n in 1..10000).
 * On invalid input output is unchanged; output must not overlap input. */
bool root_mean_squared_error(const double *target, const double *prediction,
                             size_t n, double *out);
#endif
