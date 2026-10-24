#ifndef EDUJBARRIOS_AI_LOGSUMEXP_H
#define EDUJBARRIOS_AI_LOGSUMEXP_H
#include <stdbool.h>
#include <stddef.h>
/* Finite x[n], n=1..4096. Outputs are caller-owned, sized,
   nonoverlapping, and unchanged on failure. */
bool logsumexp(const double *x,size_t n,double *value,double *gradient);
#endif
