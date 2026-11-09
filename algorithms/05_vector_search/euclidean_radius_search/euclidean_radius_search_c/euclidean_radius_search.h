#ifndef EDUJBARRIOS_AI_EUCLIDEAN_RADIUS_SEARCH_H
#define EDUJBARRIOS_AI_EUCLIDEAN_RADIUS_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
/* query[d], candidates[n*d] flat row-major, n=1..128, d=1..32.
   Coordinates finite [-1000,1000], radius finite [0,10000].
   Caller owns distinct indices[128] and count_out. On failure both
   outputs remain unchanged. Results sorted by (distance, index). */
bool euclidean_radius_search(const double *query, const double *candidates,
                             size_t n, size_t d, double radius,
                             size_t indices[128], size_t *count_out);
#endif
