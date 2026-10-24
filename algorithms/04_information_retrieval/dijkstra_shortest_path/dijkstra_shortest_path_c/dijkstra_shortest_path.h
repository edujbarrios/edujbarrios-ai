#ifndef EDUJBARRIOS_AI_DIJKSTRA_SHORTEST_PATH_H
#define EDUJBARRIOS_AI_DIJKSTRA_SHORTEST_PATH_H
#include <stdbool.h>
#include <stddef.h>
/* Dense directed graph, row-major weights[n*n]; zero means no edge.
   n=1..128; positive finite weights <=1e6; source is zero-based.
   distances[n] must be disjoint and stays untouched on error.
   Unreachable outputs use -1. */
bool dijkstra_shortest_path(const double *weights,size_t n,size_t source,
                            double *distances);
#endif
