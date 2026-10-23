#ifndef EDUJBARRIOS_AI_MINI_BATCH_GRADIENT_DESCENT_H
#define EDUJBARRIOS_AI_MINI_BATCH_GRADIENT_DESCENT_H
#include <stdbool.h>
#include <stddef.h>
/* Fit y ~= weight*x + bias with deterministic sequential batches.
   On false returns, neither output is changed. Inputs have n entries;
   output pointers must be distinct non-null locations. */
bool mini_batch_gradient_descent(
    const double *x, const double *y, size_t n,
    double learning_rate, size_t epochs, size_t batch_size,
    double *weight_out, double *bias_out);
#endif
