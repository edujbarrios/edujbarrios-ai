#ifndef EDUJBARRIOS_AI_FLOYD_WARSHALL_H
#define EDUJBARRIOS_AI_FLOYD_WARSHALL_H
#include <stdbool.h>
#include <stddef.h>
/* Row-major n*n adjacency, 1<=n<=64; +Inf is missing edge.
   Caller owns distances[n*n], disjoint from adjacency.
   On error (including negative cycle), distances is unchanged. */
bool floyd_warshall(const double *adjacency,size_t n,double *distances);
#endif
