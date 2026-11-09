#ifndef EDUJBARRIOS_AI_BINARY_PROJECTION_PROFILES_H
#define EDUJBARRIOS_AI_BINARY_PROJECTION_PROFILES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* 0/1 flat HxW input (dimensions 1..64), distinct caller-owned output
   with H+W uint16_t entries. First H row counts, last W column counts.
   Invalid data leave the output unchanged. */
bool binary_projection_profiles(
    const uint8_t *pixels, size_t height, size_t width, uint16_t *output);
#endif
