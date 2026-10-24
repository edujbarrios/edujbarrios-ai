#ifndef EDUJBARRIOS_AI_KRUSKAL_MST_H
#define EDUJBARRIOS_AI_KRUSKAL_MST_H
#include <stdbool.h>
#include <stddef.h>
/* Symmetric row-major W[n*n], +Inf absent edge, n=1..64.
   Outputs u[n-1], v[n-1], total; u/v can be NULL for n==1.
   C caller supplies correctly sized disjoint buffers, unchanged on failure. */
bool kruskal_mst(const double *weights,size_t n,
                 size_t *u,size_t *v,double *total);
#endif
