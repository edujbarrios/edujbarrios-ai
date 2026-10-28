#ifndef EDUJBARRIOS_AI_MEAN_SQUARED_LOG_ERROR_H
#define EDUJBARRIOS_AI_MEAN_SQUARED_LOG_ERROR_H
#include <stdbool.h>
#include <stddef.h>

/* MSLE for aligned nonnegative finite arrays, each value <=1e6,
 * n in 1..10000. On failure *out unchanged; no input/output overlap. */
bool mean_squared_log_error(const double *target, const double *prediction,
                            size_t n, double *out);
#endif
