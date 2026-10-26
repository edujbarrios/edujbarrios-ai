#ifndef EDUJBARRIOS_AI_EXPONENTIAL_MOVING_AVERAGE_H
#define EDUJBARRIOS_AI_EXPONENTIAL_MOVING_AVERAGE_H
#include <stdbool.h>
#include <stddef.h>
/* Nonempty finite values; alpha in (0,1]. Output must not alias input.
   On error all out[] elements remain unchanged. */
bool exponential_moving_average(const double *values, size_t n,
                                double alpha, double *out);
#endif
