#ifndef EDUJBARRIOS_AI_GRAYSCALE_CONTRAST_STRETCHING_H
#define EDUJBARRIOS_AI_GRAYSCALE_CONTRAST_STRETCHING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Flat 8-bit H*W grayscale mask with dimensions 1..64.
   Distinct output of length H*W, unchanged if validation fails. */
bool grayscale_contrast_stretching(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output);
#endif
