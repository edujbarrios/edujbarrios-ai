#ifndef EDUJBARRIOS_AI_CHEBYSHEV_DISTANCE_H
#define EDUJBARRIOS_AI_CHEBYSHEV_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>
/* On failure *out is unchanged. Arrays and output must not alias. */
bool chebyshev_distance(const double *left, const double *right,
                        size_t n, double *out);
#endif
