#ifndef EDUJBARRIOS_AI_SINGLE_LINK_CLUSTERING_H
#define EDUJBARRIOS_AI_SINGLE_LINK_CLUSTERING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Single-link clustering of 1..32 2D integer points, 1<=k<=n.
   Coordinates: flat row-major [x0,y0,...]. Output unchanged on error. */
bool single_link_clustering(const int32_t *points, size_t n, size_t k,
                            size_t *labels);
#endif
