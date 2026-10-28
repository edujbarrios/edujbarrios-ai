#ifndef EDUJBARRIOS_AI_TOPOLOGICAL_SORT_H
#define EDUJBARRIOS_AI_TOPOLOGICAL_SORT_H
#include <stdbool.h>
#include <stddef.h>

/* Kahn topological ordering, n in 1..128, directed binary adjacency.
   Output size_t[n] is left unchanged on invalid input or graph cycles;
   it must not overlap the input. */
bool topological_sort(const int *weights, size_t n, size_t *order);
#endif
