#ifndef EDUJBARRIOS_AI_BINARY_MUTUAL_INFORMATION_H
#define EDUJBARRIOS_AI_BINARY_MUTUAL_INFORMATION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Nonempty aligned binary u8 samples. The output scalar and input arrays
 * must not overlap. Output remains unchanged on failure. */
bool binary_mutual_information(const uint8_t *x, const uint8_t *y,
                               size_t n, double *result);
#endif
