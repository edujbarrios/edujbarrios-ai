#ifndef EDUJBARRIOS_AI_EUCLIDEAN_TOP_K_H
#define EDUJBARRIOS_AI_EUCLIDEAN_TOP_K_H
#include <stdbool.h>
#include <stddef.h>
/* Rank rows of candidates[n*d] in ascending Euclidean distance from
   query[d], breaking ties by original index. Caller owns indices[k].
   Input/output buffers must have capacity and must not overlap.
   On false, indices is unchanged. */
bool euclidean_top_k(const double *query,size_t d,const double *candidates,
                     size_t n,size_t k,size_t *indices);
#endif
