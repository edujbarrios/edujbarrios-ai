#ifndef EDUJBARRIOS_AI_BFS_SHORTEST_PATH_H
#define EDUJBARRIOS_AI_BFS_SHORTEST_PATH_H
#include <stdbool.h>
#include <stddef.h>

/* Directed 0/1 adjacency matrix, row-major, n in 1..128.
   On invalid input, caller-owned distances remain unchanged.
   Output must not overlap input weights. */
bool bfs_shortest_path(const int *weights, size_t n, size_t source,
                       int *distances);
#endif
