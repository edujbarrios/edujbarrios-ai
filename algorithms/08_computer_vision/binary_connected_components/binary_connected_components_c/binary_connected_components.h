#ifndef EDUJBARRIOS_AI_BINARY_CONNECTED_COMPONENTS_H
#define EDUJBARRIOS_AI_BINARY_CONNECTED_COMPONENTS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Flat binary image, dimensions 1..64, row-major labels.
   labels_out must be a distinct buffer for H*W size_t values.
   Outputs remain unchanged on validation failure. */
bool binary_connected_components(const uint8_t *pixels, size_t height, size_t width,
                                 size_t *labels_out, size_t *component_count_out);
#endif
