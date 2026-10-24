#ifndef EDUJBARRIOS_AI_MASKED_ATTENTION_H
#define EDUJBARRIOS_AI_MASKED_ATTENTION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* q[d], keys[m*d], values[m*v], mask[m] (1=allowed, 0=excluded).
   Keys/values are row-major. Caller owns disjoint, correctly sized buffers.
   out[v] is unchanged on failure. */
bool masked_attention(const double *q, size_t d, const double *keys,
                      const double *values, const uint8_t *mask,
                      size_t m, size_t v, double *out);
#endif
