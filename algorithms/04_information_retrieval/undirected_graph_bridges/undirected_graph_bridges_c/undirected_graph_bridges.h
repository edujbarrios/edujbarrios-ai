#ifndef EDUJBARRIOS_AI_UNDIRECTED_GRAPH_BRIDGES_H
#define EDUJBARRIOS_AI_UNDIRECTED_GRAPH_BRIDGES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Undirected simple n*n 0/1 adjacency, 1<=n<=64.
   Caller provides capacity 4032 endpoint values and a distinct count pointer.
   Count is number of edge pairs, not number of flattened integers.
   Invalid input leaves all outputs unchanged. */
bool undirected_graph_bridges(const uint8_t *adjacency, size_t n,
                              size_t endpoints[4032], size_t *bridge_count_out);
#endif
