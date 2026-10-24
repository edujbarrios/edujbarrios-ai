#ifndef EDUJBARRIOS_AI_MAXIMUM_INNER_PRODUCT_TOP_K_H
#define EDUJBARRIOS_AI_MAXIMUM_INNER_PRODUCT_TOP_K_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major candidates[n*d], query[d], 1<=k<=n and finite doubles.
   Caller-owned indices[k] disjoint from inputs, unchanged on failure. */
bool maximum_inner_product_top_k(const double *query,size_t d,
    const double *candidates,size_t n,size_t k,size_t *indices);
#endif
