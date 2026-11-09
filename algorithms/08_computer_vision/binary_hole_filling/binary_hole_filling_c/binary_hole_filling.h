#ifndef EDUJBARRIOS_AI_BINARY_HOLE_FILLING_H
#define EDUJBARRIOS_AI_BINARY_HOLE_FILLING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Fill four-connected enclosed zero components. Caller owns distinct H*W
   output. Invalid mask/dimensions/pointers leave output unchanged. */
bool binary_hole_filling(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output);
#endif
