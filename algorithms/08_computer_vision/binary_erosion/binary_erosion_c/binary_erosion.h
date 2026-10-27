#ifndef EDUJBARRIOS_AI_BINARY_EROSION_H
#define EDUJBARRIOS_AI_BINARY_EROSION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* 1<=height,width<=64. Input contains only 0/1; output must be
   a separate H*W buffer and remains unchanged when validation fails. */
bool binary_erosion(const uint8_t *pixels, size_t height,
                    size_t width, uint8_t *output);
#endif
