#ifndef EDUJBARRIOS_AI_BINARY_DISTANCE_TRANSFORM_H
#define EDUJBARRIOS_AI_BINARY_DISTANCE_TRANSFORM_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* 4-neighbor distance to closest in-image zero; H+W if none.
   Distinct caller-owned output of H*W u16 entries. Invalid masks and
   dimensions leave the output untouched. */
bool binary_distance_transform(
    const uint8_t *pixels, size_t height, size_t width, uint16_t *output);
#endif
