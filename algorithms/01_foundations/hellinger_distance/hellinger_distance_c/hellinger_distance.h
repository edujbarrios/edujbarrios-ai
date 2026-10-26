#ifndef EDUJBARRIOS_AI_HELLINGER_DISTANCE_H
#define EDUJBARRIOS_AI_HELLINGER_DISTANCE_H

#include <stdbool.h>
#include <stddef.h>

/* Equal-length, nonempty discrete distributions; output unchanged on error. */
bool hellinger_distance(const double *p, const double *q, size_t n, double *result);

#endif
