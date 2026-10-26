#ifndef EDUJBARRIOS_AI_TOTAL_VARIATION_DISTANCE_H
#define EDUJBARRIOS_AI_TOTAL_VARIATION_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>

/* p and q: nonempty normalized probability arrays of n elements.
   Output is left unchanged on invalid input. */
bool total_variation_distance(const double *p, const double *q,
                              size_t n, double *result);
#endif
