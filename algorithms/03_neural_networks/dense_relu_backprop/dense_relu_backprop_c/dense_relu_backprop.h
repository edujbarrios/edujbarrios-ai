#ifndef EDUJBARRIOS_AI_DENSE_RELU_BACKPROP_H
#define EDUJBARRIOS_AI_DENSE_RELU_BACKPROP_H
#include <stdbool.h>
#include <stddef.h>
/* x[d], weights[o*d] row-major, bias[o], upstream[o].
   grad_x[d], grad_weights[o*d], grad_bias[o] must be allocated and disjoint
   from all input buffers and one another. Outputs unchanged on failure. */
bool dense_relu_backprop(const double *x, size_t d, const double *weights,
                         const double *bias, const double *upstream, size_t o,
                         double *grad_x, double *grad_weights, double *grad_bias);
#endif
