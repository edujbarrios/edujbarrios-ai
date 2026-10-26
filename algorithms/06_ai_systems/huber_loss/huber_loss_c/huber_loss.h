#ifndef EDUJBARRIOS_AI_HUBER_LOSS_H
#define EDUJBARRIOS_AI_HUBER_LOSS_H
#include <stdbool.h>
#include <stddef.h>

/* Finite paired vectors of n > 0 values and positive finite delta.
   On invalid input or overflow, *result is left untouched. */
bool huber_loss(const double *actual, const double *predicted,
                size_t n, double delta, double *result);
#endif
