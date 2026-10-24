#ifndef EDUJBARRIOS_AI_PAGERANK_H
#define EDUJBARRIOS_AI_PAGERANK_H
#include <stdbool.h>
#include <stddef.h>
/* Dense directed weighted graph W[n*n], row-major, n=1..64.
   Each weight finite in [0,1e6]. Damping=0.85, 100 steps.
   Caller-provided scores[n] unchanged on any error and disjoint from W. */
bool pagerank(const double *weights,size_t n,double *scores);
#endif
