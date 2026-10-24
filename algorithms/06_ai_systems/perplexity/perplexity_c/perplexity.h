#ifndef EDUJBARRIOS_AI_PERPLEXITY_H
#define EDUJBARRIOS_AI_PERPLEXITY_H
#include <stdbool.h>
#include <stddef.h>
/* Finite natural log probabilities <=0, 1<=n<=4096.
   Caller owns result scalar disjoint from input; unchanged on false. */
bool perplexity(const double *log_probabilities,size_t n,double *result);
#endif
