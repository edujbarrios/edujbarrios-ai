#ifndef HAMMING_DISTANCE_H
#define HAMMING_DISTANCE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Count differing byte positions; output remains unchanged on failure. */
bool hamming_distance(const uint8_t *a, size_t len_a, const uint8_t *b,
                      size_t len_b, size_t *distance);

#endif
