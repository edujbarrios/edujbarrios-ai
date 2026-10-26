#ifndef EDUJBARRIOS_AI_MANHATTAN_DISTANCE_H
#define EDUJBARRIOS_AI_MANHATTAN_DISTANCE_H
#include <stdbool.h>
#include <stddef.h>
/* On failure *out remains unchanged; arrays and output must not alias. */
bool manhattan_distance(const double *left, const double *right,
                        size_t n, double *out);
#endif
