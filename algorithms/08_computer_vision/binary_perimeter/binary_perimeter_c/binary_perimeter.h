#ifndef EDUJBARRIOS_AI_BINARY_PERIMETER_H
#define EDUJBARRIOS_AI_BINARY_PERIMETER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Unit-edge perimeter of flat row-major 0/1 mask, dimensions 1..64.
   Outside the image counts as background. Invalid inputs leave perimeter
   unchanged. Output must be distinct from the image buffer. */
bool binary_perimeter(
    const uint8_t *pixels, size_t height, size_t width, uint32_t *perimeter_out);
#endif
