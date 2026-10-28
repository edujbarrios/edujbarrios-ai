#ifndef EDUJBARRIOS_AI_BIPARTITE_GRAPH_CHECK_H
#define EDUJBARRIOS_AI_BIPARTITE_GRAPH_CHECK_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Validate 1..128-vertex undirected binary adjacency and output
 * bipartiteness. Invalid input returns false without changing *out;
 * output must not overlap input. Odd cycles return true with *out=false. */
bool bipartite_graph_check(const uint8_t *adjacency, size_t n, bool *out);
#endif
