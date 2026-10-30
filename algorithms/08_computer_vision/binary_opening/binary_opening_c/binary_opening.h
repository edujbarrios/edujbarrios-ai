#ifndef BINARY_OPENING_H
#define BINARY_OPENING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Row-major image; output can alias input exactly. On failure output unchanged. */
bool binary_opening(const uint8_t *image, size_t n, size_t width,
                    size_t height, uint8_t *output);
#endif
