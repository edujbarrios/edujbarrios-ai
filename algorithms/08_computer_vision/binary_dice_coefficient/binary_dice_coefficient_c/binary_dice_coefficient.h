#ifndef BINARY_DICE_COEFFICIENT_H
#define BINARY_DICE_COEFFICIENT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* On failure *score is unchanged. Empty foreground in both masks scores 1. */
bool binary_dice_coefficient(const uint8_t *prediction, size_t n_prediction,
                             const uint8_t *target, size_t n_target, double *score);
#endif
