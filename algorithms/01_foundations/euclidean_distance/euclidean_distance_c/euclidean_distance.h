#ifndef EDUJBARRIOS_AI_EUCLIDEAN_DISTANCE_H
#define EDUJBARRIOS_AI_EUCLIDEAN_DISTANCE_H

#include <stdbool.h>
#include <stddef.h>

/* Stable L2 distance of two finite real arrays.
   Caller guarantees each array contains at least n doubles.
   A null array is valid for n == 0 only. *out is unchanged on failure. */
bool euclidean_distance(const double *left, const double *right,
                        size_t n, double *out);

#endif
