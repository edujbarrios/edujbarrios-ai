#ifndef EDUJBARRIOS_AI_SOBEL_EDGES_H
#define EDUJBARRIOS_AI_SOBEL_EDGES_H
#include <stdbool.h>
#include <stddef.h>
/* Flat row-major pixels, 1<=height,width<=64; caller-owned output
   is distinct from pixels and unchanged if validation fails. */
bool sobel_edges(const double *pixels, size_t height, size_t width,
                 double *magnitude);
#endif
