#ifndef EDUJBARRIOS_AI_Z_SCORE_STANDARDIZATION_H
#define EDUJBARRIOS_AI_Z_SCORE_STANDARDIZATION_H
#include <stdbool.h>
#include <stddef.h>
/* x[n*d] row-major, n>=1,d>=1. Caller owns means[d], stds[d],
   normalized[n*d]; all output buffers disjoint from each other and inputs.
   On error no caller-owned outputs are modified. */
bool z_score_standardization(const double *x,size_t n,size_t d,
    double *means,double *stds,double *normalized);
#endif
