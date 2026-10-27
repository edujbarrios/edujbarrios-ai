#ifndef EDUJBARRIOS_AI_BELLMAN_FORD_SHORTEST_PATH_H
#define EDUJBARRIOS_AI_BELLMAN_FORD_SHORTEST_PATH_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t from;
    size_t to;
    double weight;
} BFEdge;

/* Directed graph with 1..128 vertices and up to 16384 edges.
   Returns false for invalid inputs or reachable negative cycles.
   Output is unchanged on failure and must not overlap edges. */
bool bellman_ford_shortest_path(const BFEdge *edges, size_t m, size_t n,
                                size_t source, double *distances);
#endif
