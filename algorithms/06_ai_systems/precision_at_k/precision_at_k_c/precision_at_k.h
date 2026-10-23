#ifndef EDUJBARRIOS_AI_PRECISION_AT_K_H
#define EDUJBARRIOS_AI_PRECISION_AT_K_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Binary relevance array of n items, k in [1,n]. Caller owns all buffers.
   On error, *out is unchanged. */
bool precision_at_k(const uint8_t *relevance, size_t n, size_t k, double *out);
#endif
