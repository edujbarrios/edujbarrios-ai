#ifndef EDUJBARRIOS_AI_SCALED_DOT_PRODUCT_ATTENTION_H
#define EDUJBARRIOS_AI_SCALED_DOT_PRODUCT_ATTENTION_H
#include <stdbool.h>
#include <stddef.h>
/* q[d], keys[m*d] row-major, values[m*v] row-major, output[v].
   All buffers must meet declared capacities, output disjoint from inputs.
   Output is unchanged when false is returned. */
bool scaled_dot_product_attention(
    const double *q, size_t d, const double *keys,
    const double *values, size_t m, size_t v, double *output);
#endif
