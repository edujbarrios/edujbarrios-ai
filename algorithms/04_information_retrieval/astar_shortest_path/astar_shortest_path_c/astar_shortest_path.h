#ifndef EDUJBARRIOS_AI_ASTAR_SHORTEST_PATH_H
#define EDUJBARRIOS_AI_ASTAR_SHORTEST_PATH_H
#include <stdbool.h>
#include <stddef.h>
/* Directed row-major W[n*n], heuristic[n] consistent, goal h=0,
   1<=n<=64, source/target in range. Caller owns distance scalar,
   path[n] and path_length scalar, all disjoint from input and
   unchanged on error. Unreachable => distance=-1, path_length=0. */
bool astar_shortest_path(const double *weights,const double *heuristic,
    size_t n,size_t source,size_t target,double *distance,
    size_t *path,size_t *path_length);
#endif
