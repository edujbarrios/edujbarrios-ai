#ifndef EDUJBARRIOS_AI_SHANNON_ENTROPY_H
#define EDUJBARRIOS_AI_SHANNON_ENTROPY_H
#include <stdbool.h>
#include <stddef.h>
/* Finite probabilities[n] in [0,1] summing to one ±1e-9, 1<=n<=4096.
   Caller-owned result disjoint from input, preserved on failure. */
bool shannon_entropy(const double *probabilities,size_t n,double *result);
#endif
