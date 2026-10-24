#ifndef EDUJBARRIOS_AI_AVERAGE_PRECISION_AT_K_H
#define EDUJBARRIOS_AI_AVERAGE_PRECISION_AT_K_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* relevance[n] contains 0/1 values; n>=1, 1<=k<=n.
   total_relevant is corpus-wide (>= sum(relevance)) and <= INT32_MAX.
   output scalar is unchanged on failure, and must not alias input. */
bool average_precision_at_k(const uint8_t *relevance,size_t n,size_t k,
                            size_t total_relevant,double *result);
#endif
