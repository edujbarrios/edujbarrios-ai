#ifndef EDUJBARRIOS_AI_GRAYSCALE_HISTOGRAM_EQUALIZATION_H
#define EDUJBARRIOS_AI_GRAYSCALE_HISTOGRAM_EQUALIZATION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Flat row-major H*W 8-bit image, dimensions 1..64.
   Distinct caller-owned H*W output, unchanged on validation failure. */
bool grayscale_histogram_equalization(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output);
#endif
