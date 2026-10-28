#ifndef EDUJBARRIOS_AI_STRONGLY_CONNECTED_COMPONENTS_H
#define EDUJBARRIOS_AI_STRONGLY_CONNECTED_COMPONENTS_H
#include <stdbool.h>
#include <stddef.h>

/* SCC labels are the minimum vertex ID within the component.
   n in 1..128; directed binary row-major matrix.
   Output must not overlap input and is unchanged on invalid input. */
bool strongly_connected_components(const int *adjacency, size_t n,
                                   size_t *labels);
#endif
