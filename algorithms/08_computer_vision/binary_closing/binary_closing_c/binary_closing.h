#ifndef BINARY_CLOSING_H
#define BINARY_CLOSING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Exact in-place output is allowed. Failure leaves output unchanged. */
bool binary_closing(const uint8_t *image, size_t n, size_t width, size_t height, uint8_t *output);
#endif
