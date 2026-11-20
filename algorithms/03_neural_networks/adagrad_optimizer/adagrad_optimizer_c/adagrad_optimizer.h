#ifndef EDUJBARRIOS_AI_ADAGRAD_OPTIMIZER_H
#define EDUJBARRIOS_AI_ADAGRAD_OPTIMIZER_H
#include <stdbool.h>
#include <stddef.h>

/* One AdaGrad step with finite aligned doubles, nonnegative accumulators,
 * strictly positive learning rate and epsilon. All pointers nonnull,
 * even for n=0; input and output buffers must not overlap.
 * Both outputs remain unchanged on failure. */
bool adagrad_optimizer(const double *parameters, const double *gradients,
                       const double *accumulator, size_t n,
                       double learning_rate, double epsilon,
                       double *next_parameters, double *next_accumulator);
#endif
