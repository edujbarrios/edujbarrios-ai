#ifndef EDUJBARRIOS_AI_LABEL_SMOOTHED_CROSS_ENTROPY_H
#define EDUJBARRIOS_AI_LABEL_SMOOTHED_CROSS_ENTROPY_H
#include <stdbool.h>
#include <stddef.h>
/* 1<=n<=4096, finite logits[n], 0<=target<n, 0<=epsilon<=1.
   Caller-owned loss scalar and gradients[n] are disjoint from inputs.
   Both outputs remain unchanged on any failure. */
bool label_smoothed_cross_entropy(const double *logits,size_t n,
    size_t target,double epsilon,double *loss,double *gradients);
#endif
