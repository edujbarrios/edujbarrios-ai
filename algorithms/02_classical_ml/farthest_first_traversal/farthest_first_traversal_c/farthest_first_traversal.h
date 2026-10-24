#ifndef EDUJBARRIOS_AI_FARTHEST_FIRST_TRAVERSAL_H
#define EDUJBARRIOS_AI_FARTHEST_FIRST_TRAVERSAL_H
#include <stdbool.h>
#include <stddef.h>
/* Select k distinct center indices in greedy order, starting at 0.
   Input points[n*d] is row-major. indices[k] is caller-owned, correctly
   sized, non-overlapping, and unchanged on failure. */
bool farthest_first_traversal(const double *points,size_t n,size_t d,
                              size_t k,size_t *indices);
#endif
