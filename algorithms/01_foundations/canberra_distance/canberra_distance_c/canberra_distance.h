#ifndef EDUJBARRIOS_AI_CANBERRA_DISTANCE_H
#define EDUJBARRIOS_AI_CANBERRA_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>

/* Nonempty, equal-length finite coordinate arrays.
   On failure, *result is not modified. */
bool canberra_distance(const double *a, const double *b,
                       size_t n, double *result);
#endif
