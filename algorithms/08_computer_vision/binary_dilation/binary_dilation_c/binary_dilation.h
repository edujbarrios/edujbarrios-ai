#ifndef EDUJBARRIOS_AI_BINARY_DILATION_H
#define EDUJBARRIOS_AI_BINARY_DILATION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Flat 0/1 bytes, H,W in 1..64. Distinct caller-owned H*W output;
   output is unchanged on validation failure. */
bool binary_dilation(const uint8_t *pixels, size_t height,
                     size_t width, uint8_t *output);
#endif
