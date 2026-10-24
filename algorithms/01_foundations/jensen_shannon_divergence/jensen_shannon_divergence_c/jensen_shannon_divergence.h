#ifndef EDUJBARRIOS_AI_JENSEN_SHANNON_DIVERGENCE_H
#define EDUJBARRIOS_AI_JENSEN_SHANNON_DIVERGENCE_H
#include <stdbool.h>
#include <stddef.h>
/* p[n] and q[n] are finite probability vectors summing to 1 (+/- 1e-9).
   n in 1..4096. Outputs unchanged on error, no input/output aliasing. */
bool jensen_shannon_divergence(const double *p,const double *q,
                               size_t n,double *result);
#endif
