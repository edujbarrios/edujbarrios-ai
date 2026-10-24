#ifndef EDUJBARRIOS_AI_EXACT_COSINE_TOP_K_H
#define EDUJBARRIOS_AI_EXACT_COSINE_TOP_K_H
#include <stdbool.h>
#include <stddef.h>
/* query[d], candidates[n*d] row-major, indices[k] caller-owned.
   n,d>=1, 1<=k<=n. Non-overlapping buffers with correct capacities.
   Failure never writes to indices. */
bool exact_cosine_top_k(const double *query, size_t d,
                        const double *candidates, size_t n,
                        size_t k, size_t *indices);
#endif
