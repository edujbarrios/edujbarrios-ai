#ifndef EDUJBARRIOS_AI_MINKOWSKI_DISTANCE_H
#define EDUJBARRIOS_AI_MINKOWSKI_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>

/* Finite coordinates, n > 0, finite p >= 1; unchanged result on failure. */
bool minkowski_distance(const double *a, const double *b,
                        size_t n, double p, double *result);
#endif
