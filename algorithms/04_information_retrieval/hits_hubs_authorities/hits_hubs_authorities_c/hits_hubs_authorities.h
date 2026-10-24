#ifndef EDUJBARRIOS_AI_HITS_HUBS_AUTHORITIES_H
#define EDUJBARRIOS_AI_HITS_HUBS_AUTHORITIES_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major nonnegative weighted directed adjacency[n*n] for n=1..64.
   100 iterations with L2 normalization of authorities and hubs.
   Distinct outputs [n] remain unchanged on error; do not alias input. */
bool hits_hubs_authorities(const double *weights,size_t n,
    double *authorities,double *hubs);
#endif
