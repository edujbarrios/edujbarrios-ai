#ifndef EDUJBARRIOS_AI_INTEGRAL_IMAGE_H
#define EDUJBARRIOS_AI_INTEGRAL_IMAGE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Input: distinct row-major H*W uint8_t pixels, H,W in 1..64.
   output: caller-owned H*W uint32_t prefix sums.
   On validation failure, output is unchanged. */
bool integral_image(const uint8_t *pixels, size_t height, size_t width,
                    uint32_t *output);
#endif
