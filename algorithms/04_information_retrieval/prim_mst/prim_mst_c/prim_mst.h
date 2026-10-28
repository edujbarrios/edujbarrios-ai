#ifndef EDUJBARRIOS_AI_PRIM_MST_H
#define EDUJBARRIOS_AI_PRIM_MST_H
#include <stdbool.h>
#include <stddef.h>

/* Minimum spanning tree total weight on a connected undirected graph.
   n in 1..128, row-major matrix, symmetric positive edges, 0 absent.
   On failure, *total is unchanged. total must not overlap weights. */
bool prim_mst(const double *weights, size_t n, double *total);
#endif
