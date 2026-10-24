#ifndef EDUJBARRIOS_AI_GELU_H
#define EDUJBARRIOS_AI_GELU_H
#include <stdbool.h>
#include <stddef.h>
/* Evaluate tanh-approximated GELU and its derivative over finite x[n].
   Caller-provided output arrays are disjoint and unchanged on error. */
bool gelu(const double *x,size_t n,double *values,double *derivatives);
#endif
