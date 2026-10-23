#ifndef EDUJBARRIOS_AI_GRADIENT_DESCENT_H
#define EDUJBARRIOS_AI_GRADIENT_DESCENT_H
#include <stdbool.h>
#include <stddef.h>

/* Exactly epochs full-batch MSE updates. x/y have at least n items.
   Output pointers must be distinct and remain unchanged on error. */
bool gradient_descent(const double *x, const double *y, size_t n,
                      double learning_rate, size_t epochs,
                      double *weight_out, double *bias_out);
#endif
