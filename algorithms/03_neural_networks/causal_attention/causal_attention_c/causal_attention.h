#ifndef EDUJBARRIOS_AI_CAUSAL_ATTENTION_H
#define EDUJBARRIOS_AI_CAUSAL_ATTENTION_H
#include <stdbool.h>
#include <stddef.h>
/* Q,K shape [T*d]; V/output shape [T*v]; all row-major.
   Caller guarantees sufficient disjoint buffers; output unchanged on error. */
bool causal_attention(const double *queries, const double *keys,
                      const double *values, size_t t, size_t d,
                      size_t v, double *output);
#endif
