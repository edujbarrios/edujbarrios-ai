#ifndef EDUJBARRIOS_AI_BINARY_BOUNDARY_EXTRACTION_H
#define EDUJBARRIOS_AI_BINARY_BOUNDARY_EXTRACTION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Extract 4-neighbor foreground boundary pixels in flat 0/1 images.
   H and W in 1..64. Output must be distinct, length H*W and unchanged
   on invalid input. Input and output buffers must not overlap. */
bool binary_boundary_extraction(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output);
#endif
