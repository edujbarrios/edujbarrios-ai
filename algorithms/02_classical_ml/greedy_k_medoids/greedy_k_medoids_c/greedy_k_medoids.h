#ifndef EDUJBARRIOS_AI_GREEDY_K_MEDOIDS_H
#define EDUJBARRIOS_AI_GREEDY_K_MEDOIDS_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major X[n*d], 1<=n<=128,1<=d<=32,1<=k<=n.
   Caller owns medoids[k] disjoint from input; unchanged on failure.
   Exact ties choose lowest unselected zero-based index. */
bool greedy_k_medoids(const double *points,size_t n,size_t d,size_t k,
                      size_t *medoids);
#endif
