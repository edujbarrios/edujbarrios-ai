#ifndef EDUJBARRIOS_AI_UNDIRECTED_CONNECTED_COMPONENTS_H
#define EDUJBARRIOS_AI_UNDIRECTED_CONNECTED_COMPONENTS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Minimum vertex label of each undirected component, n in 1..128.
   Input: row-major, symmetric binary adjacency. Output is not modified
   on invalid input and must not overlap the input. */
bool undirected_connected_components(const uint8_t *adjacency,
                                      size_t n, size_t *labels);
#endif
