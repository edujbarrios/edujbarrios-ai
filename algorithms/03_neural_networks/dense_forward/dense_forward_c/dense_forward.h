#ifndef EDUJBARRIOS_AI_DENSE_FORWARD_H
#define EDUJBARRIOS_AI_DENSE_FORWARD_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major weights [output_dim x input_dim], biases [output_dim].
   Input, weights, bias, output buffers must hold their declared sizes;
   output must not overlap inputs. Output stays unchanged on failure. */
bool dense_forward(const double *input, size_t input_dim,
                   const double *weights, const double *bias, size_t output_dim,
                   double *output);
#endif
