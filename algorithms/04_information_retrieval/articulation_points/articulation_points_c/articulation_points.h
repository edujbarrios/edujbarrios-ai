#ifndef EDUJBARRIOS_AI_ARTICULATION_POINTS_H
#define EDUJBARRIOS_AI_ARTICULATION_POINTS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Undirected loop-free 0/1 row-major n*n adjacency, 1<=n<=64.
   Caller provides output capacity 64, count pointer distinct from output.
   Invalid inputs leave both outputs unchanged. */
bool articulation_points(
    const uint8_t *adjacency, size_t n, size_t cut_vertices[64],
    size_t *count_out);
#endif
